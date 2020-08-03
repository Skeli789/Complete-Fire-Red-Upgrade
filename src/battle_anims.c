#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/battle_interface.h"
#include "../include/battle_gfx_sfx_util.h"
#include "../include/decompress.h"
#include "../include/event_data.h"
#include "../include/gpu_regs.h"
#include "../include/random.h"
#include "../include/sound.h"

#include "../include/constants/songs.h"

#include "../include/new/battle_anims.h"
#include "../include/new/battle_terrain.h"
#include "../include/new/battle_util.h"
#include "../include/new/dns.h"
#include "../include/new/dynamax.h"
#include "../include/new/mega.h"
/*
battle_anims.c
	Functions and structures to modify attack animations.
*/


#define gMonFrontPicTable ((const struct CompressedSpriteSheet*) *((u32*) 0x8000128))
#define gMonBackPicTable ((const struct CompressedSpriteSheet*) *((u32*) 0x800012C))
#define gMonFrontPicCoords ((const struct MonCoords*) *((u32*) 0x8011F4C))
#define gMonBackPicCoords ((const struct MonCoords*) *((u32*) 0x8074634))

extern const u8* sBattleAnimScriptPtr;
extern u16 sAnimMoveIndex;
extern s8 gAnimFramesToWait;
extern u16 sAnimMoveIndex;

extern const struct CompressedSpriteSheet gBattleAnimPicTable[];
extern const struct CompressedSpritePalette gBattleAnimPaletteTable[];
extern const u8* const gMoveAnimations[];
extern const u8* const gBattleAnims_General[];

static const union AnimCmd sAnimCmdBerryChomp[] =
{
	ANIMCMD_FRAME(0, 0x30),
	ANIMCMD_FRAME(0, 0x25),
	ANIMCMD_FRAME(16, 3),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdBerryChomp[] =
{
	sAnimCmdBerryChomp,
};

static const union AnimCmd sAnimCmdBerryEaten[] =
{
	ANIMCMD_FRAME(16, 3),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdBerryEaten[] =
{
	sAnimCmdBerryEaten,
};

static const union AnimCmd sAnimCmdPowerWhipOnPlayer[] =
{
	ANIMCMD_FRAME(0, 3, .hFlip = TRUE),
	ANIMCMD_FRAME(16, 3, .hFlip = TRUE),
	ANIMCMD_FRAME(32, 3, .hFlip = TRUE),
	ANIMCMD_FRAME(48, 3, .hFlip = TRUE),
	ANIMCMD_FRAME(64, 3, .hFlip = TRUE),

	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdPowerWhipOnOpponent[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(16, 3),
	ANIMCMD_FRAME(32, 3),
	ANIMCMD_FRAME(48, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdPowerWhip[] =
{
	sAnimCmdPowerWhipOnPlayer,
	sAnimCmdPowerWhipOnOpponent,
};

static const union AnimCmd sAnimCmdQuickGuardLeft[] =
{
	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdQuickGuardRight[] =
{
	ANIMCMD_FRAME(0, 1, .hFlip = TRUE),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_QuickGuard[] =
{
	sAnimCmdQuickGuardLeft,
	sAnimCmdQuickGuardRight,
};

static const union AnimCmd sAnimCmdDreepyMissilePlayer[] =
{
	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdDreepyMissileOpponent[] =
{
	ANIMCMD_FRAME(16, 1),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_DreepyMissile[] =
{
	sAnimCmdDreepyMissilePlayer,
	sAnimCmdDreepyMissileOpponent,
};

const struct OamData gPoisonColumnOam =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x64),
	.size = SPRITE_SIZE(32x64),
	.priority = 2, //Save level as sprites
};

const struct OamData gPoisonColumnOamPlayer =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x64),
	.size = SPRITE_SIZE(32x64),
	.priority = 3, //Below sprites
};

static const union AnimCmd sAnimCmdPoisonColumn[] =
{
	ANIMCMD_FRAME(0, 6),
	ANIMCMD_FRAME(32, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(96, 3),
	ANIMCMD_FRAME(128, 3),
	ANIMCMD_FRAME(160, 3),
	ANIMCMD_FRAME(192, 3),
	ANIMCMD_JUMP(3),
};

const union AnimCmd *const gAnimCmdTable_PoisonColumn[] =
{
	sAnimCmdPoisonColumn,
};

static const union AnimCmd sAnimCmdLargeSpike[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(32, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(96, 3),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_LargeSpike[] =
{
	sAnimCmdLargeSpike,
};

static const union AffineAnimCmd sSpriteAffineAnim_LargeSpikePointedLeft[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 32, 1), //45 degree turn
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_LargeSpikePointedRight[] =
{
	AFFINEANIMCMD_FRAME(0, 0, -32, 1), //45 degree turn
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_LargeSpike[] =
{
	sSpriteAffineAnim_LargeSpikePointedLeft,
	sSpriteAffineAnim_LargeSpikePointedRight,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_SnipeShot[] =
{
	sSpriteAffineAnim_LargeSpikePointedRight,
};

static const union AffineAnimCmd sSpriteAffineAnim_LargeHailRock[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_LargeHailRock[] =
{
	sSpriteAffineAnim_LargeHailRock,
};

static const union AffineAnimCmd sSpriteAffineAnim_EvenLargerHailRock[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_FRAME(128, 128, 0, 1), //1.5x sprite size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_EvenLargerHailRock[] =
{
	sSpriteAffineAnim_EvenLargerHailRock,
};

static const union AnimCmd sAnimCmdIceRock[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(128, 3),
	ANIMCMD_FRAME(192, 3),
	ANIMCMD_FRAME(256, 3),
	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdDoNothing[] =
{
	ANIMCMD_FRAME(0, 0),
	ANIMCMD_END,
};

const union AnimCmd* const gAnimCmdTable_IceRock[] =
{
	sAnimCmdDoNothing,
	sAnimCmdIceRock,
};

static const union AnimCmd sAnimCmdIceRockMulti[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(128, 3),
	ANIMCMD_END,
};

const union AnimCmd* const gAnimCmdTable_IceRockMulti[] =
{
	sAnimCmdDoNothing,
	sAnimCmdIceRockMulti,
};

static const union AnimCmd sAnimCmdStonePillar[] =
{
	ANIMCMD_FRAME(192, 12),
	ANIMCMD_FRAME(128, 12),
	ANIMCMD_FRAME(64, 12),
	ANIMCMD_FRAME(0, 12),	
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_StonePillar[] =
{
	sAnimCmdStonePillar,
};

static const union AnimCmd sAnimCmdStonePillarMulti[] =
{
	ANIMCMD_FRAME(128, 12),
	ANIMCMD_FRAME(64, 12),
	ANIMCMD_FRAME(0, 12),	
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_StonePillarMulti[] =
{
	sAnimCmdStonePillarMulti,
};

static const union AffineAnimCmd sSpriteAffineAnim_StonePillarGrow[] =
{
	AFFINEANIMCMD_FRAME(196, 196, 0, 1), //1.75x sprite siz
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_StonePillarFallLeft[] =
{
	AFFINEANIMCMD_FRAME(196, 196, 0, 1), //1.75x sprite size
	AFFINEANIMCMD_FRAME(0, 0, 1, 0x40),
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_StonePillarFallRight[] =
{
	AFFINEANIMCMD_FRAME(196, 196, 0, 1), //1.75x sprite size
	AFFINEANIMCMD_FRAME(0, 0, -1, 0x40),
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_StonePillar[] =
{
	sSpriteAffineAnim_StonePillarGrow,
	sSpriteAffineAnim_StonePillarFallLeft,
	sSpriteAffineAnim_StonePillarFallRight,
};

static const union AffineAnimCmd sSpriteAffineAnim_VineLashLeft[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_VineLashRight[] =
{
	AFFINEANIMCMD_FRAME(-256 - 256 - 256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_VineLash[] =
{
	sSpriteAffineAnim_VineLashLeft,
	sSpriteAffineAnim_VineLashRight,
};

static const union AnimCmd sAnimCmdGrowingMushroom[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(16, 3),
	ANIMCMD_FRAME(32, 3),
	ANIMCMD_FRAME(48, 3),
	ANIMCMD_END
};

const union AnimCmd *const gAnimCmdTable_GrowingMushroom[] =
{
	sAnimCmdGrowingMushroom,
};

static const union AnimCmd sAnimCmdTornado[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(128, 3),
	ANIMCMD_JUMP(0)
};

const union AnimCmd *const gAnimCmdTable_Tornado[] =
{
	sAnimCmdTornado,
};

static const union AnimCmd sAnimCmdSnipeShot[] =
{
	ANIMCMD_FRAME(64, 4),
	ANIMCMD_FRAME(48, 4),
	ANIMCMD_FRAME(32, 4),
	ANIMCMD_FRAME(16, 4),
	ANIMCMD_FRAME(0, 4),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_SnipeShot[] =
{
	sAnimCmdSnipeShot,
};

static const union AnimCmd sAnimCmdSmallRock[] =
{
	ANIMCMD_FRAME(4, 1),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdTable_SmallRock[] =
{
	sAnimCmdSmallRock,
};

static const union AnimCmd sAnimCmdHoopaRing[] =
{
	ANIMCMD_FRAME(0, 8),
	ANIMCMD_FRAME(16, 8),
	ANIMCMD_FRAME(32, 8),
	ANIMCMD_FRAME(48, 8),
	ANIMCMD_JUMP(0)
};

const union AnimCmd *const gAnimCmdTable_HoopaRing[] =
{
	sAnimCmdHoopaRing,
};

static const union AffineAnimCmd sSpriteAffineAnim_HoopaRing[] =
{
	AFFINEANIMCMD_FRAME(4, 4, 10, 1), //Grow & Spin
	AFFINEANIMCMD_JUMP(0),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_HoopaRing[] =
{
	sSpriteAffineAnim_HoopaRing,
};

static const union AffineAnimCmd sSpriteAffineAnim_GrowingRing[] =
{
	AFFINEANIMCMD_FRAME(8, 8, 0, 16), //Double in size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_GrowingRing[] =
{
	sSpriteAffineAnim_GrowingRing,
};

static const union AffineAnimCmd sSpriteAffineAnim_IcicleCrash[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 128, 1), //180 degree turn
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_IcicleCrash[] =
{
	sSpriteAffineAnim_IcicleCrash,
};

static const union AffineAnimCmd sSpriteAffineAnim_FallingTealAlert[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 32, 1), //45 degree turn
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_FallingTealAlert[] =
{
	sSpriteAffineAnim_FallingTealAlert,
};

const struct OamData sSunsteelStrikeBlastOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(64x64),
	.size = SPRITE_SIZE(64x64),
	.priority = 1, //Above sprites
};

static const union AffineAnimCmd sSpriteAffineAnim_SunsteelStrikeBlastEnemySide[] =
{
	AFFINEANIMCMD_FRAME(0, 0, -64, 1), //90 degree turn
	AFFINEANIMCMD_FRAME(0, 0, 0, 7), //Pause
	AFFINEANIMCMD_FRAME(16, 16, 0, 15), //Double in size
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_SunsteelStrikeBlastPlayerSide[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 128, 1), //180 degree turn
	AFFINEANIMCMD_FRAME(0, 0, 0, 7), //Pause
	AFFINEANIMCMD_FRAME(16, 16, 0, 15), //Double in size
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_SunsteelStrikeBlast[] =
{
	sSpriteAffineAnim_SunsteelStrikeBlastEnemySide,
	sSpriteAffineAnim_SunsteelStrikeBlastPlayerSide,
};

static const union AffineAnimCmd sSpriteAffineAnim_HydroCannonBall[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 16), //Double in size
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_HydroCannonBall[] =
{
	sSpriteAffineAnim_HydroCannonBall,
};

static const union AffineAnimCmd sSpriteAffineAnim_AuraSphereBall[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0), //Start at 1 pixel
	AFFINEANIMCMD_FRAME(16, 16, 0, 15),

	//Pulsate
	AFFINEANIMCMD_FRAME(-8, -8, 10, 3), //Shrink & Spin
	AFFINEANIMCMD_FRAME(8, 8, 10, 3), //Grow & Spin
	AFFINEANIMCMD_JUMP(2),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_AuraSphereBall[] =
{
	sSpriteAffineAnim_AuraSphereBall,
};

const struct OamData sGrowingSuperpowerOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(64x64),
	.size = SPRITE_SIZE(64x64),
	.priority = 2,
};

static const union AffineAnimCmd sSpriteAffineAnim_GrowingSuperpowerEnemyAttack[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 128, 1), //180 degree turn
	AFFINEANIMCMD_FRAME(0, 0, 0, 2), //Pause
	AFFINEANIMCMD_FRAME(16, 16, 0, 15), //Double in size
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_GrowingSuperpowerPlayerAttack[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 0, 2), //Pause
	AFFINEANIMCMD_FRAME(16, 16, 0, 15), //Double in size
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_GrowingSuperpower[] =
{
	sSpriteAffineAnim_GrowingSuperpowerPlayerAttack,
	sSpriteAffineAnim_GrowingSuperpowerEnemyAttack,
};


static const union AffineAnimCmd sSpriteAffineAnim_CrushGripHandEnemyAttack[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 96, 1), //180 degree turn
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_DoNothing[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 0, 1), //Do nothing
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_CrushGripHand[] =
{
	sSpriteAffineAnim_DoNothing,
	sSpriteAffineAnim_CrushGripHandEnemyAttack,
};

const struct OamData sDracoMeteorRockOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1,
};

const struct OamData sDracoMeteorTailOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_BLEND,
	.shape = SPRITE_SHAPE(16x16),
	.size = SPRITE_SIZE(16x16),
	.priority = 2,
};

static const union AffineAnimCmd sSpriteAffineAnim_SearingShotRock[] =
{
	AFFINEANIMCMD_FRAME(8, 8, 9, 15),
	AFFINEANIMCMD_FRAME(-8, -8, 9, 15),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_SearingShotRock[] =
{
	sSpriteAffineAnim_SearingShotRock,
};

static const union AffineAnimCmd sSpriteAffineAnim_JudgmentBall[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0),
	AFFINEANIMCMD_FRAME(8, 8, 0, 15), //Half size
	AFFINEANIMCMD_FRAME(0, 0, 0, 120), //Delay
	AFFINEANIMCMD_FRAME(24, 24, 0, 5), //Normal size
	AFFINEANIMCMD_FRAME(0, 0, 0, 10), //Delay
	AFFINEANIMCMD_FRAME(-16, -16, 0, 15), //Revert to 1 px
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_JudgmentBall[] =
{
	sSpriteAffineAnim_JudgmentBall,
};

static const union AffineAnimCmd sSpriteAffineAnim_SpiritBreakBall[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0),
	AFFINEANIMCMD_FRAME(2, 2, 0, 50), //Grow slowly to half size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_SpiritBreakBall[] =
{
	sSpriteAffineAnim_SpiritBreakBall,
};

static const union AffineAnimCmd sSpriteAffineAnim_HalfSize[] =
{
	AFFINEANIMCMD_FRAME(-128, -128, 0, 1), //Half size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_HalfSize[] =
{
	sSpriteAffineAnim_HalfSize,
};

const struct OamData sMaxPhantasmObjectsOam =
{
	.affineMode = ST_OAM_AFFINE_NORMAL,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above sprites
};

static const union AffineAnimCmd sSpriteAffineAnim_GrowToNormalSize[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0), //Start at 1px
	AFFINEANIMCMD_FRAME(8, 8, 0, 30),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_GrowToNormalSize[] =
{
	sSpriteAffineAnim_GrowToNormalSize,
};

static const union AffineAnimCmd sSpriteAffineAnim_FeintFist[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_FRAME(-16, -16, 0, 16),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_FeintFist[] =
{
	sSpriteAffineAnim_FeintFist,
};

static const union AffineAnimCmd sSpriteAffineAnim_MachPunchFist[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_FRAME(-32, -32, 4, 8),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_MachPunchFist[] =
{
	sSpriteAffineAnim_MachPunchFist,
};

static const union AffineAnimCmd sSpriteAffineAnim_DrainPunchFist[] =
{
	AFFINEANIMCMD_FRAME(256, 256, 0, 1), //Double sprite size
	AFFINEANIMCMD_FRAME(-32, -32, 0, 8),
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_DrainPunchFist[] =
{
	sSpriteAffineAnim_DrainPunchFist,
};

static const union AffineAnimCmd sSpriteAffineAnim_MudBombBall[] =
{
	AFFINEANIMCMD_FRAME(0, -8, -4, 4), //Compress Vertically
	AFFINEANIMCMD_FRAME(-8, 8, -4, 4), //Compress Horizontally, Normalize Vertically
	AFFINEANIMCMD_FRAME(8, -8, -4, 4), //Normalize Horizontally, Compress Vertically
	AFFINEANIMCMD_JUMP(1),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_MudBombBall[] =
{
	sSpriteAffineAnim_MudBombBall,
};

static const union AffineAnimCmd sSpriteAffineAnim_NightSlashLeft[] =
{
	AFFINEANIMCMD_FRAME(32, 32, 0, 1), //Increase size by 1/4
	AFFINEANIMCMD_FRAME(0, 0, -32, 1), //Rotate
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_NightSlashLeft[] =
{
	sSpriteAffineAnim_NightSlashLeft,
};

static const union AffineAnimCmd sSpriteAffineAnim_NightSlashRight[] =
{
	AFFINEANIMCMD_FRAME(32, 32, 0, 1), //Increase size by 1/4
	AFFINEANIMCMD_FRAME(0, 0, 96, 1), //Rotate
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_NightSlashRight[] =
{
	sSpriteAffineAnim_NightSlashRight,
};

static const union AffineAnimCmd sSpriteAffineAnim_PsychoCutOpponent[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 128, 1), //180 degree turn
	AFFINEANIMCMD_FRAME(4, 4, 0, 64), //Double in size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_PsychoCutOpponent[] =
{
	sSpriteAffineAnim_PsychoCutOpponent,
};

static const union AffineAnimCmd sSpriteAffineAnim_WakeUpSlap[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 64, 1), //Rotate left 90 degrees
	AFFINEANIMCMD_FRAME(0, -32, 0, 7), //Flatten horizontally (on its side)
	AFFINEANIMCMD_FRAME(0, 0, 0, 8), //Do Nothing
	AFFINEANIMCMD_FRAME(0, -288, 0, 1), //Unflatten in other direction
	AFFINEANIMCMD_FRAME(0, 32, 0, 7), //Flatten horizontally (on its side)
	AFFINEANIMCMD_FRAME(0, 0, 0, 8), //Do Nothing
	AFFINEANIMCMD_FRAME(0, 288, 0, 1), //Unflatten in other direction
	AFFINEANIMCMD_JUMP(1),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_WakeUpSlap[] =
{
	sSpriteAffineAnim_WakeUpSlap,
};

static const union AffineAnimCmd sSpriteAffineAnim_DoubleSlap[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 64, 1), //Rotate left 90 degrees
	AFFINEANIMCMD_FRAME(0, 0, 0, 3), //Do Nothing
	AFFINEANIMCMD_FRAME(0, -28, 0, 8), //Flatten horizontally (on its side)
	AFFINEANIMCMD_FRAME(0, 0, 0, 11), //Do Nothing
	AFFINEANIMCMD_FRAME(0, -288, 0, 1), //Unflatten in other direction
	AFFINEANIMCMD_FRAME(0, 0, 0, 2), //Do Nothing
	AFFINEANIMCMD_FRAME(0, 28, 0, 8), //Flatten horizontally (on its side)
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_DoubleSlap[] =
{
	sSpriteAffineAnim_DoubleSlap,
};

static const union AffineAnimCmd sSpriteAffineAnim_WingAttackFeather[] =
{
	AFFINEANIMCMD_FRAME(0, 0, -1, 14), //Rotate a little right
	AFFINEANIMCMD_FRAME(0, 0, 1, 28), //Rotate a little left
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_WingAttackFeather[] =
{
	sSpriteAffineAnim_WingAttackFeather,
};

static const union AffineAnimCmd sSpriteAffineAnim_FlutterbyPulsate[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 4),
	AFFINEANIMCMD_FRAME(-16, -16, 0, 4),
	AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSpriteAffineAnim_FlutterbyGrow[] =
{
	AFFINEANIMCMD_FRAME(8, 8, 0, 16), //Double in size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_Flutterby[] =
{
	sSpriteAffineAnim_FlutterbyPulsate,
	sSpriteAffineAnim_FlutterbyGrow,
};

static const union AffineAnimCmd sSpriteAffineAnim_MushroomCloud[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 0),
	AFFINEANIMCMD_FRAME(32, 32, 0, 15), //Double in size
	AFFINEANIMCMD_FRAME(0, 0, 0, 48),
	AFFINEANIMCMD_FRAME(-32, -32, 0, 15), //Double in size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_MushroomCloud[] =
{
	sSpriteAffineAnim_MushroomCloud,
};

static const union AnimCmd sAnimCmdStarfallBeam[] =
{
	ANIMCMD_FRAME(0, 1),
	ANIMCMD_END
};

static const union AnimCmd sAnimCmdSmiteBeam[] =
{
	ANIMCMD_FRAME(4, 1),
	ANIMCMD_END
};

static const union AnimCmd sAnimCmdPhotonGeyserBeam[] =
{
	ANIMCMD_FRAME(8, 1),
	ANIMCMD_END
};

const union AnimCmd *const gAnimCmdTable_StarfallBeam[] =
{
	sAnimCmdStarfallBeam,
	sAnimCmdSmiteBeam,
	sAnimCmdPhotonGeyserBeam,
};

static const union AffineAnimCmd sSpriteAffineAnim_StarfallBeam[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 0, 16), //Delay
	AFFINEANIMCMD_FRAME(64, 64, 0, 4), //Double in size
	//Pulsate
	AFFINEANIMCMD_FRAME(-128, -128, 0, 1),
	AFFINEANIMCMD_FRAME(128, 128, 0, 1),
	AFFINEANIMCMD_JUMP(2),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_StarfallBeam[] =
{
	sSpriteAffineAnim_DoNothing,
	sSpriteAffineAnim_StarfallBeam,
};

//This file's functions:
static void InitSpritePosToAnimTargetsCentre(struct Sprite *sprite, bool8 respectMonPicOffsets);
static void InitSpritePosToAnimAttackersCentre(struct Sprite *sprite, bool8 respectMonPicOffsets);
static void InitSpritePosToGivenTarget(struct Sprite* sprite, u8 target);
static void SpriteCB_FlareBlitzUpFlamesP2(struct Sprite* sprite);
static void AnimMindBlownBallStep(struct Sprite *sprite);
static u8 GetProperCentredCoord(u8 bank, u8 coordType);
static void Task_HandleSpecialBattleAnimation(u8 taskId);
static bool8 ShouldAnimBeDoneRegardlessOfSubsitute(u8 animId);
static bool8 ShouldSubstituteRecedeForSpecialBattleAnim(u8 animId);
static void TrySwapBackupSpeciesWithSpecies(u8 activeBattler, u8 animId);
static void AnimTask_GrowStep(u8 taskId);
static void AnimDracoMeteorRockStep(struct Sprite *sprite);
static void AnimTask_DynamaxGrowthStep(u8 taskId);

bank_t LoadBattleAnimTarget(u8 arg)
{
	u8 battler;
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		switch (gBattleAnimArgs[arg]) {
			case 0:
				battler = gBattleAnimAttacker;
				break;
			default:
				battler = gBattleAnimTarget;
				break;
			case 2:
				battler = PARTNER(gBattleAnimAttacker);
				break;
			case 3:
				battler = PARTNER(gBattleAnimTarget);
				break;
		}
	}
	else
	{
		if (gBattleAnimArgs[arg] == 0)
			battler = gBattleAnimAttacker;
		else
			battler = gBattleAnimTarget;
	}
	return battler;
}

#define GET_TRUE_SPRITE_INDEX(i) ((i - ANIM_SPRITES_START))
void ScriptCmd_loadspritegfx(void)
{
	u16 index;

	sBattleAnimScriptPtr++;
	index = T1_READ_16(sBattleAnimScriptPtr);
	LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[GET_TRUE_SPRITE_INDEX(index)]);
	LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[GET_TRUE_SPRITE_INDEX(index)]);
	sBattleAnimScriptPtr += 2;
	AddSpriteIndex(GET_TRUE_SPRITE_INDEX(index));
	gAnimFramesToWait = 1;
	gAnimScriptCallback = WaitAnimFrameCount;
}

void LoadShinyStarsSpriteTiles(void)
{
	if (GetSpriteTileStartByTag(ANIM_TAG_GOLD_STARS) == 0xFFFF)
	{
		LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[ANIM_TAG_GOLD_STARS - ANIM_SPRITES_START]);
		LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[ANIM_TAG_GOLD_STARS - ANIM_SPRITES_START]);
	}
}

void TryStartShinyAnimation(u8 battler, unusedArg struct Pokemon* mon)
{
	u8 taskId1, taskId2;
	gBattleSpritesDataPtr->healthBoxesData[battler].flag_x80 = 1;

	if (IsBattlerSpriteVisible(battler) && IsMonShiny(GetIllusionPartyData(battler)))
	{
		LoadShinyStarsSpriteTiles();
		taskId1 = CreateTask((void*) (0x80F181C | 1), 10);
		taskId2 = CreateTask((void*) (0x80F181C | 1), 10);
		gTasks[taskId1].data[0] = battler;
		gTasks[taskId2].data[0] = battler;
		gTasks[taskId1].data[1] = 0;
		gTasks[taskId2].data[1] = 1;
		return;
	}

	gBattleSpritesDataPtr->healthBoxesData[battler].field_1_x1 = 1;
}

void AnimTask_TechnoBlast(u8 taskId)
{
	gBattleAnimArgs[0] = gItems[GetBankPartyData(gBattleAnimAttacker)->item].holdEffectParam;
	DestroyAnimVisualTask(taskId);
}

void AnimTask_GetTimeOfDay(u8 taskId)
{
	gBattleAnimArgs[0] = 0; //Daytime

	#ifdef TIME_ENABLED
		if (IsNightTime())
			gBattleAnimArgs[0] = 1;
		else if (IsEvening())
			gBattleAnimArgs[0] = 2;
	#endif

	DestroyAnimVisualTask(taskId);
}

void AnimTask_GetLycanrocForm(u8 taskId)
{
	#ifdef SPECIES_LYCANROC_N
	if (GetIllusionPartyData(gBattleAnimAttacker)->species == SPECIES_LYCANROC_N)
		gBattleAnimArgs[0] = 1;
	else
	#endif
		gBattleAnimArgs[0] = 0;

	DestroyAnimVisualTask(taskId);
}

void AnimTask_IsTargetPartner(u8 taskId)
{
	if (gBattleAnimTarget == PARTNER(gBattleAnimAttacker))
		gBattleAnimArgs[0] = 1;
	else
		gBattleAnimArgs[0] = 0;

	DestroyAnimVisualTask(taskId);
}

void AnimTask_IsRockfallPillarLoaded(u8 taskId)
{
	gBattleAnimArgs[0] = (IndexOfSpriteTileTag(ANIM_TAG_STONE_PILLAR) == 0xFF) ? 0 : 1;
	DestroyAnimVisualTask(taskId);
}

void AnimTask_IsHailstormRockLoaded(u8 taskId)
{
	gBattleAnimArgs[0] = (IndexOfSpriteTileTag(ANIM_TAG_ICE_ROCK) == 0xFF) ? 0 : 1;
	DestroyAnimVisualTask(taskId);
}

void AnimTask_GetTrappedMoveAnimId(u8 taskId)
{
	switch (gBattleSpritesDataPtr->animationData->animArg) {
		case MOVE_FIRESPIN:
		case MOVE_G_MAX_CENTIFERNO_P:
		case MOVE_G_MAX_CENTIFERNO_S:
			gBattleAnimArgs[0] = 1;
			break;
		case MOVE_WHIRLPOOL:
			gBattleAnimArgs[0] = 2;
			break;
		case MOVE_CLAMP:
			gBattleAnimArgs[0] = 3;
			break;
		case MOVE_SANDTOMB:
		case MOVE_G_MAX_SANDBLAST_P:
		case MOVE_G_MAX_SANDBLAST_S:
			gBattleAnimArgs[0] = 4;
			break;
		case MOVE_MAGMASTORM:
			gBattleAnimArgs[0] = 5;
			break;
		case MOVE_INFESTATION:
			gBattleAnimArgs[0] = 6;
			break;
		case MOVE_SNAPTRAP:
			gBattleAnimArgs[0] = 7;
			break;
		case MOVE_OCTOLOCK:
			gBattleAnimArgs[0] = 8;
			break;
		default:
			gBattleAnimArgs[0] = 0;
	}

	DestroyAnimVisualTask(taskId);
}

void AnimTask_GetRaidBattleStormLevel(u8 taskId)
{
	gBattleAnimArgs[0] = gNewBS->dynamaxData.stormLevel;
	DestroyAnimVisualTask(taskId);
}

void AnimTask_IsUnbound(u8 taskId)
{
	#ifdef UNBOUND
	gBattleAnimArgs[0] = TRUE;
	#else
	gBattleAnimArgs[0] = FALSE;
	#endif
	DestroyAnimVisualTask(taskId);
}

bool8 ShadowSneakAnimHelper(void)
{
	switch (sAnimMoveIndex) {
		case MOVE_SHADOWSNEAK:
		case MOVE_HYPERSPACEHOLE:
		case MOVE_SPECTRALTHIEF:
		case MOVE_PULVERIZING_PANCAKE:
			return TRUE;
	}

	return FALSE;
}

bool8 IsAnimMoveIonDeluge(void)
{
	return gBattleBufferA[gBattleAnimAttacker][0] == CONTROLLER_MOVEANIMATION && sAnimMoveIndex == MOVE_IONDELUGE;
}

bool8 DoesAnimMoveGetMaxRollout(void)
{
	return sAnimMoveIndex == MOVE_TECTONIC_RAGE_P || sAnimMoveIndex == MOVE_TECTONIC_RAGE_S
		|| sAnimMoveIndex == MOVE_MAX_QUAKE_P || sAnimMoveIndex == MOVE_MAX_QUAKE_S
		|| sAnimMoveIndex == MOVE_G_MAX_SANDBLAST_P || sAnimMoveIndex == MOVE_G_MAX_SANDBLAST_S;
}

bool8 IsAnimMoveBloomDoom(void)
{
	return sAnimMoveIndex == MOVE_BLOOM_DOOM_P || sAnimMoveIndex == MOVE_BLOOM_DOOM_S;
}

bool8 IsAnimMoveOceanicOperretta(void)
{
	return sAnimMoveIndex == MOVE_OCEANIC_OPERETTA;
}

bool8 DoesMoveHaveGeyserOnTarget(void)
{
	return sAnimMoveIndex == MOVE_NEVER_ENDING_NIGHTMARE_P || sAnimMoveIndex == MOVE_NEVER_ENDING_NIGHTMARE_S
		|| sAnimMoveIndex == MOVE_DEVASTATING_DRAKE_P      || sAnimMoveIndex == MOVE_DEVASTATING_DRAKE_S
		|| sAnimMoveIndex == MOVE_GIGAVOLT_HAVOC_P         || sAnimMoveIndex == MOVE_GIGAVOLT_HAVOC_S
		|| sAnimMoveIndex == MOVE_GUARDIAN_OF_ALOLA
		|| sAnimMoveIndex == MOVE_LIGHT_THAT_BURNS_THE_SKY
		|| IsAnyMaxMove(sAnimMoveIndex);
}

bool8 IsAnimMoveDestinyBond(void)
{
	return sAnimMoveIndex == MOVE_DESTINYBOND;
}

bool8 IsAnimMoveThunderWave(void)
{
	return sAnimMoveIndex == MOVE_THUNDERWAVE;
}

bool8 IsAnimMoveGrudge(void)
{
	return sAnimMoveIndex == MOVE_GRUDGE;
}

bool8 IsAnimMoveFairyLock(void)
{
	return sAnimMoveIndex == MOVE_FAIRYLOCK;
}

bool8 IsAnimMoveFlashCannonOrSteelBeam(void)
{
	return sAnimMoveIndex == MOVE_FLASHCANNON
		|| sAnimMoveIndex == MOVE_STEELBEAM;
}

bool8 IsAnimMoveSkillSwap(void)
{
	return sAnimMoveIndex == MOVE_SKILLSWAP;
}

bool8 IsAnimMovePowerSwap(void)
{
	return sAnimMoveIndex == MOVE_POWERSWAP;
}

bool8 IsAnimMoveHeartSwap(void)
{
	return sAnimMoveIndex == MOVE_HEARTSWAP;
}

bool8 IsAnimMoveMudBomb(void)
{
	return sAnimMoveIndex == MOVE_MUDBOMB;
}

bool8 IsAnimMoveCoreEnforcer(void)
{
	return sAnimMoveIndex == MOVE_COREENFORCER;
}

bool8 IsAnimMoveBulletSeed(void)
{
	return sAnimMoveIndex == MOVE_BULLETSEED;
}

bool8 IsAnimMoveKingsShield(void)
{
	return sAnimMoveIndex == MOVE_KINGSSHIELD;
}

void AnimTask_ReloadAttackerSprite(u8 taskId)
{
	u8 spriteId = gBattlerSpriteIds[gBattleAnimAttacker];

	struct Task* task = &gTasks[taskId];
	struct Task* newTask;

	switch (task->data[10]) {
		case 0:
			// To fix an annoying graphical glitch where the old sprite would flash
			// momentarily, we hide the sprite offscreen while we refresh it.
			// Remember the old position so we can go back to it later.
			task->data[11] = gSprites[spriteId].pos1.x;
			gSprites[spriteId].pos1.x = -64;

			// Load the palette and graphics. Note this doesn't cause the sprite to
			// refresh
			LoadBattleMonGfxAndAnimate(gBattleAnimAttacker, 1, spriteId);
			++task->data[10];
			break;

		case 1:
			// Actually update the sprite now
			gSprites[spriteId].invisible = FALSE;
			newTask = &gTasks[CreateTask(sub_807331C, 5)];
			newTask->data[0] = 0;
			newTask->data[2] = gBattleAnimAttacker;
			++task->data[10];
			break;

		case 2:
			// Make sure the task is done. I'm not sure if this is necessary
			if (!FuncIsActiveTask(sub_807331C))
				++task->data[10];
			break;

		case 3:
			// Restore the old X position and end the task
			gSprites[spriteId].pos1.x = task->data[11];
			DestroyAnimVisualTask(taskId);
	}
}

static void AnimTask_WaitAttackerCry(u8 taskId)
{
	if (!IsCryPlaying())
	{
		ClearPokemonCrySongs(); //Reset memory state
		DestroyAnimVisualTask(taskId);
	}
}

void AnimTask_PlayAttackerCry(u8 taskId)
{
	s8 pan;

	if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
		pan = -25;
	else
		pan = 25;

	PlayCry3(GetIllusionPartyData(gBattleAnimAttacker)->species, pan, 0);
	gTasks[taskId].func = AnimTask_WaitAttackerCry;	
}

u8 ModifyMegaCries(u16 species, u8 mode)
{
	if (mode <= 1)
	{
		#ifdef HIGH_PITCH_MEGA_PRIMAL_CRY
		if (IsMegaSpecies(species)
		|| IsBluePrimalSpecies(species)
		|| IsRedPrimalSpecies(species)
		|| IsGigantamaxSpecies(species))
		{
			mode = 3;
		}
		#endif
	}

	return mode;
}

void AnimTask_GetSecretPowerAnimation(u8 taskId)
{
	u16 move;

	switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
			move = gTerrainTable[0].secretPowerAnim;
			break;
		case GRASSY_TERRAIN:
			move = gTerrainTable[1].secretPowerAnim;
			break;
		case MISTY_TERRAIN:
			move = gTerrainTable[2].secretPowerAnim;
			break;
		case PSYCHIC_TERRAIN:
			move = gTerrainTable[3].secretPowerAnim;
			break;
		default:
			if (IsTerrainMoveIndoors())
				move = gTerrainTable[BATTLE_TERRAIN_INSIDE + 4].secretPowerAnim;
			else
				move = gTerrainTable[gBattleTerrain + 4].secretPowerAnim;
	}

	sBattleAnimScriptPtr = gMoveAnimations[move];
	DestroyAnimVisualTask(taskId);
}

void AnimTask_SetCamouflageBlend(u8 taskId)
{
	u8 entry = 0;
	u32 selectedPalettes = UnpackSelectedBattleAnimPalettes(gBattleAnimArgs[0]);

	switch (gTerrainType) {
		case ELECTRIC_TERRAIN:
			entry = 0;
			break;
		case GRASSY_TERRAIN:
			entry = 1;
			break;
		case MISTY_TERRAIN:
			entry = 2;
			break;
		case PSYCHIC_TERRAIN:
			entry = 3;
			break;
	}

	if (entry)
		gBattleAnimArgs[4] = gCamouflageColours[gTerrainTable[entry].camouflageType];
	else if (IsTerrainMoveIndoors())
		gBattleAnimArgs[4] = gCamouflageColours[gTerrainTable[BATTLE_TERRAIN_INSIDE + 4].camouflageType];
	else
		gBattleAnimArgs[4] = gCamouflageColours[gTerrainTable[gBattleTerrain + 4].camouflageType];

	StartBlendAnimSpriteColor(taskId, selectedPalettes);
}

void SpriteCB_TranslateAnimSpriteToTargetMonLocationDestroyMatrix(struct Sprite *sprite)
{
	bool8 v1;
	u8 coordType;

	if (!(gBattleAnimArgs[5] & 0xff00))
		v1 = TRUE;
	else
		v1 = FALSE;

	if (!(gBattleAnimArgs[5] & 0xff))
		coordType = BATTLER_COORD_Y_PIC_OFFSET;
	else
		coordType = BATTLER_COORD_Y;

	InitSpritePosToAnimAttacker(sprite, v1);
	if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, coordType) + gBattleAnimArgs[3];
	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
}

void SpriteCB_TranslateAnimSpriteToTargetMonLocationDoubles(struct Sprite* sprite)
{
	bool8 v1;
	bank_t target;
	u8 coordType;

	if (!(gBattleAnimArgs[5] & 0xff00))
		v1 = TRUE;
	else
		v1 = FALSE;

	if (!(gBattleAnimArgs[5] & 0xff))
		coordType = BATTLER_COORD_Y_PIC_OFFSET;
	else
		coordType = BATTLER_COORD_Y;

	InitSpritePosToAnimAttacker(sprite, v1);
	if (SIDE(gBattleAnimAttacker) != B_SIDE_PLAYER)
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	target = LoadBattleAnimTarget(6);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->data[0] = gBattleAnimArgs[4];
		sprite->data[2] = GetBattlerSpriteCoord(target, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
		sprite->data[4] = GetBattlerSpriteCoord(target, coordType) + gBattleAnimArgs[3];
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}

static void InitSpritePosToAnimTargetsCentre(struct Sprite *sprite, bool8 respectMonPicOffsets)
{
	if (!respectMonPicOffsets)
	{
		sprite->pos1.x = (GetBattlerSpriteCoord2(gBattleAnimTarget, BATTLER_COORD_X)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimTarget), BATTLER_COORD_X)) / 2;
		sprite->pos1.y = (GetBattlerSpriteCoord2(gBattleAnimTarget, BATTLER_COORD_Y)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimTarget), BATTLER_COORD_Y)) / 2;
	}

	SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
	sprite->pos1.y += gBattleAnimArgs[1];
}

static void InitSpritePosToAnimAttackersCentre(struct Sprite *sprite, bool8 respectMonPicOffsets)
{
	if (!respectMonPicOffsets)
	{
		sprite->pos1.x = (GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_X)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimAttacker), BATTLER_COORD_X)) / 2;
		sprite->pos1.y = (GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_Y)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimAttacker), BATTLER_COORD_Y)) / 2;
	}
	else
	{
		sprite->pos1.x = (GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_X_2)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimAttacker), BATTLER_COORD_X_2)) / 2;
		sprite->pos1.y = (GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimAttacker), BATTLER_COORD_Y_PIC_OFFSET)) / 2;
	}

	SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
	sprite->pos1.y += gBattleAnimArgs[1];
}

void SpriteCB_SpriteToCentreOfSide(struct Sprite* sprite)
{
	bool8 var;

	if (!sprite->data[0])
	{
		if (!gBattleAnimArgs[3])
			var = TRUE;
		else
			var = FALSE;

		if (gBattleAnimArgs[2] == 0) //Attacker
		{
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
				InitSpritePosToAnimAttackersCentre(sprite, var);
			else
				InitSpritePosToAnimAttacker(sprite, var);
		}
		else
		{
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
				InitSpritePosToAnimTargetsCentre(sprite, var);
			else
				InitSpritePosToAnimTarget(sprite, var);
		}

		sprite->data[0]++;
	}
	else if (sprite->animEnded || sprite->affineAnimEnded)
	{
		DestroySpriteAndMatrix(sprite);
	}
}

void SpriteCB_RandomCentredHits(struct Sprite* sprite)
{
	if (gBattleAnimArgs[1] == -1)
		gBattleAnimArgs[1] = Random() & 3;

	StartSpriteAffineAnim(sprite, gBattleAnimArgs[1]);

	if (gBattleAnimArgs[0] == 0)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			InitSpritePosToAnimAttackersCentre(sprite, FALSE);
		else
			InitSpritePosToAnimAttacker(sprite, FALSE);
	}
	else
	{
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			InitSpritePosToAnimTargetsCentre(sprite, FALSE);
		else
			InitSpritePosToAnimTarget(sprite, FALSE);
	}

	sprite->pos2.x += (Random() % 48) - 24;
	sprite->pos2.y += (Random() % 24) - 12;

	StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
	sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
}

void SpriteCB_CentredElectricity(struct Sprite* sprite)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		InitSpritePosToAnimTargetsCentre(sprite, FALSE);
	else
		InitSpritePosToAnimTarget(sprite, FALSE);

	sprite->oam.tileNum += gBattleAnimArgs[3] * 4;

	if (gBattleAnimArgs[3] == 1)
		sprite->oam.matrixNum = 8;
	else if (gBattleAnimArgs[3] == 2)
		sprite->oam.matrixNum = 16;

	sprite->data[0] = gBattleAnimArgs[2];
	sprite->callback = WaitAnimForDuration;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

void SpriteCB_CentredSpiderWeb(struct Sprite* sprite)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		InitSpritePosToAnimTargetsCentre(sprite, FALSE);
	else
		InitSpritePosToAnimTarget(sprite, FALSE);

	sprite->callback = SpriteCB_SpiderWeb;
}

static void InitSpritePosToGivenTarget(struct Sprite* sprite, u8 target)
{
	sprite->pos1.x = GetBattlerSpriteCoord2(target, BATTLER_COORD_X);
	sprite->pos1.y = GetBattlerSpriteCoord2(target, BATTLER_COORD_Y);

	SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
	sprite->pos2.y = gBattleAnimArgs[1];
}

static void InitSpritePosToGivenTargetRespectPicOffsets(struct Sprite* sprite, u8 target)
{
	sprite->pos1.x = GetBattlerSpriteCoord2(target, BATTLER_COORD_X_2);
	sprite->pos1.y = GetBattlerSpriteCoord2(target, BATTLER_COORD_Y_PIC_OFFSET);

	SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
	sprite->pos2.y = gBattleAnimArgs[1];
}

void SpriteCB_SearingShotRock(struct Sprite* sprite)
{
	u8 target = LoadBattleAnimTarget(4);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		InitSpritePosToGivenTarget(sprite, target);

		StartSpriteAnim(sprite, gBattleAnimArgs[2]);
		sprite->data[0] = gBattleAnimArgs[3];

		sprite->callback = WaitAnimForDuration;
		StoreSpriteCallbackInData6(sprite, AnimSpinningKickOrPunchFinish);
	}
}

void SpriteCB_CoreEnforcerHits(struct Sprite* sprite)
{
	StartSpriteAffineAnim(sprite, gBattleAnimArgs[3]);

	if (gBattleAnimArgs[2] == 0)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			InitSpritePosToAnimAttackersCentre(sprite, FALSE);
		else
			InitSpritePosToAnimAttacker(sprite, FALSE);
	}
	else
	{
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			InitSpritePosToAnimTargetsCentre(sprite, FALSE);
		else
			InitSpritePosToAnimTarget(sprite, FALSE);
	}

	sprite->pos1.y += 20;
	sprite->callback = SpriteCB_80BA7BC;
}

void SpriteCB_CoreEnforcerBeam(struct Sprite* sprite)
{
	if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
		SpriteCB_AnimSolarbeamBigOrb(sprite);
	else
	{
		InitSpritePosToAnimAttacker(sprite, TRUE);
		StartSpriteAnim(sprite, gBattleAnimArgs[3]);

		sprite->data[0] = gBattleAnimArgs[2];

		sprite->data[2] = (GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2)
						+  GetBattlerSpriteCoord(PARTNER(gBattleAnimTarget), BATTLER_COORD_X_2)) / 2;


		sprite->data[4] = (GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET)
						+  GetBattlerSpriteCoord(PARTNER(gBattleAnimTarget), BATTLER_COORD_Y_PIC_OFFSET)) / 2;

		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}

void CoreEnforcerLoadBeamTarget(struct Sprite* sprite)
{
	sprite->data[0] = gBattleAnimArgs[2];
	sprite->data[1] = sprite->pos1.x;
	sprite->data[2] = (GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2)
					+  GetBattlerSpriteCoord(PARTNER(gBattleAnimTarget), BATTLER_COORD_X_2)) / 2;
	sprite->data[3] = sprite->pos1.y;
	sprite->data[4] = (GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET)
					+  GetBattlerSpriteCoord(PARTNER(gBattleAnimTarget), BATTLER_COORD_Y_PIC_OFFSET)) / 2;
}


void SpriteCB_FlareBlitzUpFlames(struct Sprite* sprite)
{
	if (gBattleAnimArgs[0] == 0)
	{
		sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 0) + gBattleAnimArgs[1];
		sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 1) + gBattleAnimArgs[2];
	}
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimTarget, 0) + gBattleAnimArgs[1];
		sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimTarget, 1) + gBattleAnimArgs[2];
	}

	sprite->data[0] = 0;
	sprite->data[1] = gBattleAnimArgs[3];
	sprite->callback = SpriteCB_FlareBlitzUpFlamesP2;
}

static void SpriteCB_FlareBlitzUpFlamesP2(struct Sprite* sprite)
{
	if (++sprite->data[0] > sprite->data[1])
	{
		sprite->data[0] = 0;
		sprite->pos1.y -= 2;
	}

	sprite->pos1.y -= sprite->data[0];
	if (sprite->pos1.y < 0)
		DestroyAnimSprite(sprite);
}

#define ITEM_TAG ANIM_TAG_ITEM_BAG
u8 __attribute__((long_call)) AddItemIconSprite(u16 tilesTag, u16 paletteTag, u16 itemId);
void AnimTask_CreateFlingItem(u8 taskId)
{
	u8 iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gLastUsedItem);

	if (iconSpriteId != MAX_SPRITES)
	{
		gSprites[iconSpriteId].oam.priority = 0; //Highest priority
		gSprites[iconSpriteId].callback = (void*) 0x80B4495;
		++gAnimVisualTaskCount;
	}

	DestroyAnimVisualTask(taskId);
}

void AnimTask_CreateStealItem(u8 taskId)
{
	u8 iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gLastUsedItem);

	if (iconSpriteId != MAX_SPRITES)
	{
		struct Sprite* sprite = &gSprites[iconSpriteId];

		sprite->oam.priority = 2;
		sprite->oam.affineMode = ST_OAM_AFFINE_NORMAL;
		sprite->affineAnims = (void*) 0x83E2E80;
		sprite->callback = (void*) 0x80A36B5;

		CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, sprite->oam.affineMode);
		InitSpriteAffineAnim(sprite);
		++gAnimVisualTaskCount;
	}
	DestroyAnimVisualTask(taskId);
}

void AnimTask_CreateKnockOffItem(u8 taskId)
{
	u8 iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gLastUsedItem);

	if (iconSpriteId != MAX_SPRITES)
	{
		struct Sprite* sprite = &gSprites[iconSpriteId];

		sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
		sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
		sprite->oam.priority = 2;
		sprite->oam.affineMode = ST_OAM_AFFINE_NORMAL;
		sprite->affineAnims = (void*) 0x83E2E80;
		sprite->callback = (void*) 0x80A35F5;

		CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, sprite->oam.affineMode);
		InitSpriteAffineAnim(sprite);
		++gAnimVisualTaskCount;
	}
	DestroyAnimVisualTask(taskId);
}

void AnimTask_CreateBestowItem(u8 taskId)
{
	u8 iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gLastUsedItem);

	if (iconSpriteId != MAX_SPRITES)
	{
		gSprites[iconSpriteId].oam.priority = 2;
		gSprites[iconSpriteId].callback = (void*) 0x8075E81;
		++gAnimVisualTaskCount;
	}
	DestroyAnimVisualTask(taskId);
}

void UpdatedAnimStealItemFinalCallback(struct Sprite* sprite)
{
	sprite->data[0]++;
	if (sprite->data[0] > 50)
		DestroyAnimSprite(sprite);
}

void SpriteCB_SunsteelStrikeRings(struct Sprite* sprite)
{
	if (SIDE(gBattleAnimAttacker) != B_SIDE_PLAYER)
	{
		sprite->pos1.x = 272;
		sprite->pos1.y = -32;
	}
	else
	{
		sprite->pos1.x = -32;
		sprite->pos1.y = -32;
	}

	sprite->data[0] = gBattleAnimArgs[0];
	sprite->data[1] = sprite->pos1.x;
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
	sprite->data[3] = sprite->pos1.y;
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);

	InitAnimLinearTranslation(sprite);
	sprite->callback = (void*) 0x80B1CC1;
}

//Spins a sprite towards the target, pausing in the middle.
//Used in Mind Blown.
//arg 0: duration step 1 (attacker -> center)
//arg 1: duration step 2 (spin center)
//arg 2: duration step 3 (center -> target)
void SpriteCB_MindBlownBall(struct Sprite *sprite)
{
	s16 oldPosX = sprite->pos1.x;
	s16 oldPosY = sprite->pos1.y;
	sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
	sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 3);
	sprite->data[0] = 0;
	sprite->data[1] = gBattleAnimArgs[0];
	sprite->data[2] = gBattleAnimArgs[1];
	sprite->data[3] = gBattleAnimArgs[2];
	sprite->data[4] = sprite->pos1.x << 4;
	sprite->data[5] = sprite->pos1.y << 4;
	sprite->data[6] = ((oldPosX - sprite->pos1.x) << 4) / (gBattleAnimArgs[0] << 1);
	sprite->data[7] = ((oldPosY - sprite->pos1.y) << 4) / (gBattleAnimArgs[0] << 1);
	sprite->callback = AnimMindBlownBallStep;
}

static u8 GetProperCentredCoord(u8 bank, u8 coordType)
{
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		return (GetBattlerSpriteCoord2(bank, coordType)
			  +  GetBattlerSpriteCoord2(PARTNER(bank), coordType)) / 2;
	}

	return GetBattlerSpriteCoord(bank, coordType);
}

static void AnimMindBlownBallStep(struct Sprite *sprite)
{
	switch (sprite->data[0])
	{
	case 0:
		sprite->data[4] += sprite->data[6];
		sprite->data[5] += sprite->data[7];
		sprite->pos1.x = sprite->data[4] >> 4;
		sprite->pos1.y = sprite->data[5] >> 4;
		sprite->data[1] -= 1;
		if (sprite->data[1] > 0)
			break;
		sprite->data[0] += 1;
		break;
	case 1:
		sprite->data[2] -= 1;
		if (sprite->data[2] > 0)
			break;

		sprite->data[1] = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
		sprite->data[2] = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
		sprite->data[4] = sprite->pos1.x << 4;
		sprite->data[5] = sprite->pos1.y << 4;
		sprite->data[6] = ((sprite->data[1] - sprite->pos1.x) << 4) / sprite->data[3];
		sprite->data[7] = ((sprite->data[2] - sprite->pos1.y) << 4) / sprite->data[3];
		sprite->data[0] += 1;
		break;
	case 2:
		sprite->data[4] += sprite->data[6];
		sprite->data[5] += sprite->data[7];
		sprite->pos1.x = sprite->data[4] >> 4;
		sprite->pos1.y = sprite->data[5] >> 4;
		sprite->data[3] -= 1;
		if (sprite->data[3] > 0)
			break;
		sprite->pos1.x = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
		sprite->pos1.y = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
		sprite->data[0] += 1;
		break;
	case 3:
		DestroySpriteAndMatrix(sprite);
		break;
	}
}

void SpriteCB_MindBlownExplosion(struct Sprite* sprite)
{
	u8 a;
	u8 b;
	u16 x;
	u16 y;

	if (gBattleAnimArgs[4] == 0)
	{
		DestroyAnimSprite(sprite);
	}
	else
	{
		a = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
		b = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);

		sprite->data[0] = gBattleAnimArgs[4];
		if (gBattleAnimArgs[1] == 0)
		{
			sprite->pos1.x = gBattleAnimArgs[2] + a;
			sprite->pos1.y = gBattleAnimArgs[3] + b;
			sprite->data[5] = a;
			sprite->data[6] = b;
		}
		else
		{
			sprite->pos1.x = a;
			sprite->pos1.y = b;
			sprite->data[5] = gBattleAnimArgs[2] + a;
			sprite->data[6] = gBattleAnimArgs[3] + b;
		}

		x = sprite->pos1.x;
		sprite->data[1] = x * 16;
		y = sprite->pos1.y;
		sprite->data[2] = y * 16;
		sprite->data[3] = (sprite->data[5] - sprite->pos1.x) * 16 / gBattleAnimArgs[4];
		sprite->data[4] = (sprite->data[6] - sprite->pos1.y) * 16 / gBattleAnimArgs[4];

		sprite->callback = (void*) 0x80A43A1;
	}
}

void SpriteCB_HydroCannonImpact(struct Sprite *sprite)
{
	if (gBattleAnimArgs[2] == 0)
		InitSpritePosToAnimAttacker(sprite, 1);
	else
		InitSpritePosToAnimTarget(sprite, TRUE);

	sprite->callback = DestroyAnimSprite;
}

void SpriteCB_SoulStealingStar(struct Sprite *sprite)
{
	sprite->pos1.x += gBattleAnimArgs[0];
	sprite->pos1.y += gBattleAnimArgs[1];
	sprite->data[0] = gBattleAnimArgs[3];
	sprite->data[1] = gBattleAnimArgs[4];
	sprite->data[2] = gBattleAnimArgs[5];
	sprite->callback = (void*) 0x80B7C11;
}

void SpriteCB_GrowingSuperpower(struct Sprite *sprite)
{
	u8 battler;

	if (gBattleAnimArgs[0] == 0)
	{
		sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
		sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 3);
		battler = gBattleAnimTarget;
		sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimAttacker);
	}
	else
	{
		battler = gBattleAnimAttacker;
		sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimTarget);
	}

	if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
		StartSpriteAffineAnim(sprite, 1);

	sprite->data[0] = 16;
	sprite->data[1] = sprite->pos1.x;
	sprite->data[2] = GetBattlerSpriteCoord(battler, 2);
	sprite->data[3] = sprite->pos1.y;
	sprite->data[4] = GetBattlerSpriteCoord(battler, 3);

	InitAnimLinearTranslation(sprite);
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	sprite->callback = (void*) 0x807563D;
}

extern const struct SpriteTemplate gDracoMeteorTailSpriteTemplate;
static void AnimDracoMeteorRockStep(struct Sprite *sprite)
{
	sprite->pos2.x = ((sprite->data[2] - sprite->data[0]) * sprite->data[5]) / sprite->data[4];
	sprite->pos2.y = ((sprite->data[3] - sprite->data[1]) * sprite->data[5]) / sprite->data[4];

	if (sprite->data[5] == sprite->data[4])
		DestroyAnimSprite(sprite);

	sprite->data[5]++;
}

// Moves a shooting star across the screen that leaves little twinkling stars behind its path.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: destination x pixel offset
// arg 3: destination y pixel offset
// arg 4: duration
void SpriteCB_DracoMeteorRock(struct Sprite *sprite)
{
	if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
	{
		sprite->data[0] = sprite->pos1.x - gBattleAnimArgs[0];
		sprite->data[2] = sprite->pos1.x - gBattleAnimArgs[2];
	}
	else
	{
		sprite->data[0] = sprite->pos1.x + gBattleAnimArgs[0];
		sprite->data[2] = sprite->pos1.x + gBattleAnimArgs[2];
	}

	sprite->data[1] = sprite->pos1.y + gBattleAnimArgs[1];
	sprite->data[3] = sprite->pos1.y + gBattleAnimArgs[3];
	sprite->data[4] = gBattleAnimArgs[4];

	sprite->data[6] = gBattleAnimArgs[2];
	sprite->data[7] = gBattleAnimArgs[3];

	sprite->pos1.x = sprite->data[0];
	sprite->pos1.y = sprite->data[1];
	sprite->callback = AnimDracoMeteorRockStep;
}

const struct OamData sPyroBallRockOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(16x16),
	.size = SPRITE_SIZE(16x16),
	.priority = 1, //Above sprites
};

const struct OamData sPyroBallFlamesOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above sprites
};

//Creates a rock that bounces between the attacker's feet.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: Ignore horizontal motion if TRUE. Only bounce once.
#define sVerticalTime sprite->data[0]
#define sHorizontalTime sprite->data[1]
#define sMovingBackHorizontally sprite->data[2]
void SpriteCB_PyroBallRockBounceStep(struct Sprite* sprite)
{
	if (sprite->pos2.y > 0) //Rock returned back down
	{
		if (gBattleAnimArgs[2] || sMovingBackHorizontally) //Finished moving from left to right
			DestroyAnimSprite(sprite);
		else
		{
			sVerticalTime = 0;
			sMovingBackHorizontally = TRUE;
		}
	}

	s8 initialVerticalVelocity = -6;
	s8 initialHorizontalVelocity = -1; //Starts by moving right to left

	//vi = -6
	//g = 0.25 (gravity not negative b/c go up to go closer to 0)
	//h = vi*t + 0.5g*t^2
	//t = sVerticalTime
	sprite->pos2.y = (initialVerticalVelocity * sVerticalTime + (1 * sVerticalTime * sVerticalTime) / 4);

	//x = vi*t
	//vi = sprite->data[2]
	//t = sHorizontalTime
	if (!gBattleAnimArgs[2])
		sprite->pos2.x = (initialHorizontalVelocity * sHorizontalTime);

	sVerticalTime++;

	if (sMovingBackHorizontally)
		sHorizontalTime--; //Move left to right
	else
		sHorizontalTime++; //Move right to left
}

static void InitSpritePositionForPyroBall(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, 0);
	sprite->pos1.y += 20; //Move closer to attacker's feet

	if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
		sprite->pos1.y += 20; //Move below the text box

}

void SpriteCB_PyroBallRockBounce(struct Sprite* sprite)
{
	InitSpritePositionForPyroBall(sprite);
	sprite->callback = SpriteCB_PyroBallRockBounceStep;
}
#undef sVerticalTime
#undef sHorizontalTime
#undef sMovingBackHorizontally

//Launches a projectile from the attacker's feet at the target.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: target x pixel offset
//arg 3: target y pixel offset
//arg 4: duration
//arg 5: wave amplitude
void SpriteCB_PyroBallLaunch(struct Sprite* sprite)
{
	InitSpritePositionForPyroBall(sprite);

	if (SIDE(gBattleAnimAttacker))
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[5];
	InitAnimArcTranslation(sprite);

	sprite->callback = SpriteCB_AnimMissileArcStep;
}

const struct OamData sAppleOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above sprites
};

const struct OamData sTransparentFistOAM =
{
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_BLEND,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above sprites
};

static const union AffineAnimCmd sSpriteAffineAnim_ScaledApple[] =
{
	AFFINEANIMCMD_FRAME(32, 32, 0, 10), //Double in size
	AFFINEANIMCMD_END
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_ScaledApple[] =
{
	sSpriteAffineAnim_ScaledApple,
};

static void SpriteCB_FallingObjectStep(struct Sprite *sprite)
{
	switch (sprite->data[0])
	{
	case 0:
		sprite->pos2.y += sprite->data[3];
		if (sprite->pos2.y >= 0)
		{
			sprite->pos2.y = 0;
			sprite->data[0]++;
		}
		break;
	case 1:
		if (++sprite->data[1] > 0)
		{
			sprite->data[1] = 0;
			sprite->invisible ^= 1;
			if (++sprite->data[2] == 10)
				DestroySpriteAndMatrix(sprite);
		}
		break;
	}
}

//Causes an object to fall from the sky.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: speed
//arg 3: anim bank
void SpriteCB_FallingObject(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(3);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord2(target, BATTLER_COORD_X);
		sprite->pos1.y = GetBattlerSpriteCoord2(target, BATTLER_COORD_Y);

		sprite->pos2.x = gBattleAnimArgs[0];
		sprite->pos1.y = gBattleAnimArgs[1];
		sprite->pos2.y = -gBattleAnimArgs[1];

		if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
		{
			sprite->pos1.y += 45;
			sprite->pos2.y -= 45;
		}

		sprite->data[3] = gBattleAnimArgs[2]; //Speed
		sprite->callback = SpriteCB_FallingObjectStep;
	}
}

static void SpriteCB_FallingObjectPlayAnimOnEndStep(struct Sprite *sprite)
{
	switch (sprite->data[0])
	{
	case 0:
		sprite->pos2.y += sprite->data[3];
		if (sprite->pos2.y >= 0)
		{
			sprite->pos2.y = 0;
			sprite->data[0]++;
		}
		break;
	case 1:
		if (++sprite->data[1] > 0)
		{
			StartSpriteAnim(sprite, 1);
			sprite->callback = RunStoredCallbackWhenAnimEnds;
			StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
		}
		break;
	}
}

//Causes an object to fall from the sky.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: speed
//arg 3: anim bank
void SpriteCB_FallingObjectPlayAnimOnEnd(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(3);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord2(target, BATTLER_COORD_X);
		sprite->pos1.y = GetBattlerSpriteCoord2(target, BATTLER_COORD_Y);

		sprite->pos2.x = gBattleAnimArgs[0];
		sprite->pos1.y = gBattleAnimArgs[1];
		sprite->pos2.y = -gBattleAnimArgs[1];

		if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
		{
			sprite->pos1.y += 45;
			sprite->pos2.y -= 45;
		}

		sprite->data[3] = gBattleAnimArgs[2]; //Speed
		sprite->callback = SpriteCB_FallingObjectPlayAnimOnEndStep;
	}
}

//Causes an object to fall from the sky on the centre of the side.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: speed
void SpriteCB_FallingObjectOnTargetCentre(struct Sprite *sprite)
{
	sprite->pos1.x = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_X);
	sprite->pos1.y = GetProperCentredCoord(gBattleAnimTarget, BATTLER_COORD_Y);

	sprite->pos2.x = gBattleAnimArgs[0];
	sprite->pos1.y = gBattleAnimArgs[1];
	sprite->pos2.y = -gBattleAnimArgs[1];

	if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
	{
		sprite->pos1.y += 45;
		sprite->pos2.y -= 45;
	}

	sprite->data[3] = gBattleAnimArgs[2]; //Speed
	sprite->callback = SpriteCB_FallingObjectStep;
}

//Throws acid at a single target.
void SpriteCB_AcidLaunchSingleTarget(struct Sprite *sprite)
{
	s16 l1, l2;
	if (!gBattleAnimArgs[3])
		StartSpriteAnim(sprite, 2);

	InitSpritePosToAnimTarget(sprite, TRUE);
	l1 = sprite->pos1.x; l2 = sprite->pos1.y;
	InitSpritePosToAnimAttacker(sprite, TRUE);

	if (SIDE(gBattleAnimAttacker))
		gBattleAnimArgs[4] = -gBattleAnimArgs[4];

	sprite->data[0] = gBattleAnimArgs[2];
	sprite->data[2] = l1 + gBattleAnimArgs[4];
	sprite->data[4] = l2 + gBattleAnimArgs[5];
	sprite->data[5] = -30;

	InitAnimArcTranslation(sprite);

	sprite->callback = (void*) (0x80B1728 | 1);
}

//Causes acid to drip down a single target.
void SpriteCB_AcidDripSingleTarget(struct Sprite *sprite)
{
	if (SIDE(gBattleAnimAttacker) != B_SIDE_PLAYER)
		gBattleAnimArgs[0] = -gBattleAnimArgs[0];

	sprite->pos1.x += gBattleAnimArgs[0];
	sprite->pos1.y += gBattleAnimArgs[1];

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = sprite->pos1.x + gBattleAnimArgs[2];
	sprite->data[4] = sprite->pos1.y + sprite->data[0];

	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

static void SpriteCB_LockingJawFinish(struct Sprite *sprite)
{
	if (--sprite->data[3] <= sprite->data[6])
		DestroySpriteAndMatrix(sprite);
}

static void SpriteCB_LockingJawStep(struct Sprite *sprite)
{
	sprite->data[4] += sprite->data[0];
	sprite->data[5] += sprite->data[1];
	sprite->pos2.x = sprite->data[4] >> 8;
	sprite->pos2.y = sprite->data[5] >> 8;
	if (++sprite->data[3] == sprite->data[2])
		sprite->callback = SpriteCB_LockingJawFinish;
}

//Creates a jaw that bites down and locks on the target.
//args: Idk same as bite and crunch
//arg 6: Time to hold bite for.
void SpriteCB_LockingJaw(struct Sprite *sprite)
{
	sprite->pos1.x += gBattleAnimArgs[0];
	sprite->pos1.y += gBattleAnimArgs[1];
	StartSpriteAffineAnim(sprite, gBattleAnimArgs[2]);
	sprite->data[0] = gBattleAnimArgs[3];
	sprite->data[1] = gBattleAnimArgs[4];
	sprite->data[2] = gBattleAnimArgs[5];
	sprite->data[6] = -gBattleAnimArgs[6];
	sprite->callback = SpriteCB_LockingJawStep;
}

//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: target x pixel offset
//arg 3: target y pixel offset
//arg 4: duration
//arg 5: wave amplitude
void SpriteCB_MoongeistCharge(struct Sprite *sprite)
{
	sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2) + gBattleAnimArgs[0];
	sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[1];

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[5];

	InitAnimArcTranslation(sprite);
	sprite->callback = SpriteCB_AnimMissileArcStep;
}

static const union AnimCmd sAnimCmdWaterDropletDrip[] =
{
	ANIMCMD_FRAME(0, 3),
	ANIMCMD_FRAME(16, 3),
	ANIMCMD_FRAME(32, 3),
	ANIMCMD_FRAME(48, 3),
	ANIMCMD_END,
};

static const union AnimCmd sAnimCmdWaterDropletSplash[] =
{
	ANIMCMD_FRAME(64, 3),
	ANIMCMD_FRAME(80, 3),
	ANIMCMD_FRAME(96, 3),
	ANIMCMD_FRAME(112, 10),
	ANIMCMD_END,
};

const union AnimCmd *const gAnimCmdWaterDroplet[] =
{
	sAnimCmdWaterDropletDrip,
	sAnimCmdWaterDropletSplash,
};

static void SpriteCB_WaterDropletDrop(struct Sprite *sprite)
{
	sprite->pos2.y += 4;
	if (sprite->pos2.y >= sprite->data[1])
	{
		sprite->data[0] = 19;
		StartSpriteAnim(sprite, 1);
		sprite->callback = WaitAnimForDuration;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}

static void SpriteCB_WaterDropletDrip(struct Sprite *sprite)
{
	sprite->data[0] = 12;
	StartSpriteAnim(sprite, 0);
	sprite->callback = WaitAnimForDuration;
	StoreSpriteCallbackInData6(sprite, SpriteCB_WaterDropletDrop);
}

//Causes an object to fall from the sky.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: anim bank
void SpriteCB_WaterDroplet(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(2);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		InitSpritePosToGivenTarget(sprite, target);
		sprite->data[1] = sprite->pos1.y; //Target's y coord

		sprite->pos2.x = gBattleAnimArgs[0];
		sprite->pos2.y = 0;

		//Put droplet at the top of the screen
		sprite->pos1.y = 15;

		sprite->callback = SpriteCB_WaterDropletDrip;
	}
}

void SpriteCB_AnimSpriteOnSelectedMonPos(struct Sprite *sprite)
{
	if (!sprite->data[0])
	{
		u8 target = LoadBattleAnimTarget(2);

		if (!IsBattlerSpriteVisible(target))
			DestroyAnimSprite(sprite);
		else
		{
			InitSpritePosToGivenTarget(sprite, target);
			sprite->data[0]++;
		}
	}
	else if (sprite->animEnded || sprite->affineAnimEnded)
	{
		DestroySpriteAndMatrix(sprite);
	}
}

//Like the screen from Brick Break
void SpriteCB_SpriteOnMonForDuration(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(0);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord(target, BATTLER_COORD_X);
		sprite->pos1.y = GetBattlerSpriteCoord(target, BATTLER_COORD_Y);

		sprite->pos1.x += gBattleAnimArgs[1];
		sprite->pos1.y += gBattleAnimArgs[2];
		sprite->data[0] = 0;
		sprite->data[1] = gBattleAnimArgs[3];
		sprite->data[2] = gBattleAnimArgs[4];
		sprite->data[3] = 0;
		sprite->callback = (void*) (0x80B0EF0 | 1);
	}
}

void SpriteCB_SpriteOnMonForDurationUseY(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(0);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord(target, BATTLER_COORD_X);
		sprite->pos1.y = GetBattlerSpriteCoord(target, BATTLER_COORD_Y_PIC_OFFSET);

		sprite->pos2.x = gBattleAnimArgs[1];
		sprite->pos2.y = gBattleAnimArgs[2];
		sprite->data[0] = 0;
		sprite->data[1] = gBattleAnimArgs[3];
		sprite->data[2] = gBattleAnimArgs[4];
		sprite->data[3] = 0;
		sprite->callback = (void*) (0x80B0EF0 | 1);
	}
}

const struct OamData sFishiousRendTeethOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(64x64),
	.size = SPRITE_SIZE(64x64),
	.priority = 1, //Above sprites
};

void SpriteCB_LeftRightSliceStep1(struct Sprite *sprite)
{
	sprite->pos2.x += sprite->data[1];

	if (sprite->pos2.x >= sprite->data[0])
		DestroyAnimSprite(sprite);
}

void SpriteCB_LeftRightSliceStep0(struct Sprite *sprite)
{
	sprite->pos2.x -= sprite->data[1];

	if (sprite->pos2.x <= sprite->data[0])
	{
		sprite->data[0] = -sprite->data[0];
		sprite->callback = SpriteCB_LeftRightSliceStep1;
	}
}

//Creates a sprite that moves left then right along the target.
//arg 0: Slice distance
//arg 1: Speed
void SpriteCB_LeftRightSlice(struct Sprite *sprite)
{
	sprite->pos2.x = gBattleAnimArgs[0];
	sprite->data[0] = -gBattleAnimArgs[0]; //Slice distance
	sprite->data[1] = gBattleAnimArgs[1]; //Slice speed
	sprite->callback = SpriteCB_LeftRightSliceStep0;
}

void SpriteCB_StoneEdgeRockStep2(struct Sprite* sprite)
{
	if (--sprite->data[1] > 0) //Timer
		sprite->pos1.y -= 4; //Rise up
	else
		DestroyAnimSprite(sprite);
}

void SpriteCB_StoneEdgeRockStep1(struct Sprite* sprite)
{
	if (--sprite->data[0] > 0) //Timer
	{
		if ((sprite->data[0] & 3) == 0)
			++sprite->pos1.y; //Initial descent
	}
	else
		sprite->callback = SpriteCB_StoneEdgeRockStep2;
}

//Creates a rock for Stone Edge
//arg 0: Initial x-offset
//arg 1: Initial y-offset
void SpriteCB_StoneEdgeRock(struct Sprite* sprite)
{
	InitSpritePosToAnimTarget(sprite, TRUE);

	sprite->data[0] = 20; //Timer
	sprite->data[1] = 20;

	sprite->callback = SpriteCB_StoneEdgeRockStep1;
}

void SpriteCB_ForcePalmStep5(struct Sprite* sprite)
{
	if (++sprite->data[0] >= 6)
		DestroyAnimSprite(sprite);
}

void SpriteCB_ForcePalmStep4(struct Sprite* sprite)
{
	if (++sprite->data[0] >= 8)
	{
		sprite->data[0] = 0;
		sprite->callback = SpriteCB_ForcePalmStep5;
	}
	else
		sprite->pos1.x += 1;
}

void SpriteCB_ForcePalmStep3(struct Sprite* sprite)
{
	if (++sprite->data[0] >= 8)
	{
		sprite->data[0] = 0;
		sprite->callback = SpriteCB_ForcePalmStep4;
	}
	else
		sprite->pos1.x -= 1;
}

void SpriteCB_ForcePalmStep2(struct Sprite* sprite)
{
	sprite->data[0] = 0;
	sprite->callback = SpriteCB_ForcePalmStep3;
}

void SpriteCB_ForcePalmStep1(struct Sprite* sprite)
{
	if (++sprite->data[6] >= sprite->data[5])
	{
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, SpriteCB_ForcePalmStep2);
	}
}

//Creates an object on the target that moves linearly in some direction.
//arg 0: Initial x-offset
//arg 1: Initial y-offset
//arg 2: Final x-offset
//arg 3: Final y-offset
//arg 4: Motion speed delay
//arg 5: Pause before motion
void SpriteCB_ForcePalm(struct Sprite* sprite)
{
	InitSpritePosToAnimTarget(sprite, TRUE);

	sprite->data[0] = gBattleAnimArgs[4]; //Speed delay
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];

	sprite->data[5] = gBattleAnimArgs[5]; //Pause before motion
	sprite->data[6] = 0;
	sprite->callback = SpriteCB_ForcePalmStep1;
}

//Creates a sprite that moves right then then along the target.
//arg 0: Swipe distance
//arg 1: Speed
//arg 2: Less swipes (for Double Slap)
void SpriteCB_WakeUpSlap(struct Sprite *sprite)
{
	switch (sprite->data[7]) { //State
		case 0:
			sprite->pos2.x = gBattleAnimArgs[0];
			sprite->data[0] = -gBattleAnimArgs[0]; //Swipe distance
			sprite->data[1] = gBattleAnimArgs[1]; //Swipe speed
			sprite->data[6] = gBattleAnimArgs[2]; //Less Slaps
			++sprite->data[7];
			break;
		//Right
		case 1:
		case 3:
			sprite->pos2.x += sprite->data[1];

			if (sprite->pos2.x >= sprite->data[0])
			{
				sprite->data[0] = -sprite->data[0];
				++sprite->data[7];
			}
			break;
		//Left
		case 2:
		case 4:
			sprite->pos2.x -= sprite->data[1];

			if (sprite->pos2.x <= sprite->data[0])
			{
				sprite->data[0] = -sprite->data[0];

				if (sprite->data[6])
					sprite->data[7] = 5; //Destroy
				else
					++sprite->data[7];
			}
			break;
		case 5:
			DestroyAnimSprite(sprite);
			break;
	}
}

//Destroys the chains for Fairy Lock
void SpriteCB_FairyLockChain(struct Sprite *sprite)
{
	if (++sprite->data[0] > 12)
	{
		--gTasks[sprite->data[6]].data[sprite->data[7]];
		DestroySprite(sprite);
	}
}

void SpriteCB_SurroundingRing(struct Sprite *sprite)
{
	sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 0);
	sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 1) + 40;

	sprite->data[0] = 13;
	sprite->data[2] = sprite->pos1.x;
	sprite->data[4] = sprite->pos1.y - 72;

	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

void SpriteCB_ToxicThreadWrap(struct Sprite *sprite)
{
	if (SIDE(gBattleAnimAttacker) != B_SIDE_PLAYER)
		sprite->pos1.x -= gBattleAnimArgs[0];
	else
		sprite->pos1.x += gBattleAnimArgs[0];

	sprite->pos1.y += gBattleAnimArgs[1];
	if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
		sprite->pos1.y += 8;

	sprite->callback = (void*) (0x80B4274 | 1);
}

void SpriteCB_HorizontalSliceStep(struct Sprite *sprite)
{
	if (sprite->data[2] == 1) //Move left
		sprite->pos2.x -= sprite->data[1];
	else
		sprite->pos2.x += sprite->data[1];

	sprite->data[3] += sprite->data[1];

	if (sprite->data[3] >= sprite->data[0])
		DestroySpriteAndMatrix(sprite);
}

//Creates a sprite that moves left or right along the target.
//arg 0: Initial x-pixel offset
//arg 1: Initial y-pixel offset
//arg 2: Slice distance
//arg 3: Speed
//arg 4: direction
void SpriteCB_HorizontalSlice(struct Sprite *sprite)
{
	sprite->pos2.x = gBattleAnimArgs[0];
	sprite->pos2.y = gBattleAnimArgs[1];

	sprite->data[0] = gBattleAnimArgs[2]; //Slice distance
	sprite->data[1] = gBattleAnimArgs[3]; //Slice speed
	sprite->data[2] = gBattleAnimArgs[4]; //Slice direction
	sprite->data[3] = 0; //Timer

	sprite->callback = SpriteCB_HorizontalSliceStep;
}

//Creates the Block X on the chosen target
//arg 0: Initial x-pixel offset
//arg 1: Initial y-pixel offset
//arg 2: Target
void SpriteCB_BlockXOnTarget(struct Sprite *sprite)
{
	s16 y;
	u8 target = LoadBattleAnimTarget(2);

	if (SIDE(target) == B_SIDE_PLAYER)
	{
		sprite->subpriority = GetBattlerSpriteSubpriority(target) - 2;
		y = -144;
	}
	else
	{
		sprite->subpriority = GetBattlerSpriteSubpriority(target) + 2;
		y = -96;
	}

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		InitSpritePosToGivenTarget(sprite, target);
		sprite->pos2.y = y;
		sprite->callback = (void*) (0x80E3534 | 1); //AnimBlockX_Step;
	}
}

//Creates a bolt of lightning on the designated target
//arg 0: Target
void AnimTask_TargetedLightning(u8 taskId)
{
	struct Task *task = &gTasks[taskId];
	u8 target = LoadBattleAnimTarget(0);

	if (!IsBattlerSpriteVisible(target))
	{
		DestroyAnimVisualTask(taskId);
		return;
	}

	switch (task->data[0]) {
		case 0:
			task->data[15] = GetBattlerSpriteCoord(target, BATTLER_COORD_Y) + 32;
			task->data[14] = task->data[15];
			while (task->data[14] > 16)
				task->data[14] -= 32;
			task->data[13] = GetBattlerSpriteCoord(target, BATTLER_COORD_X_2);
			task->data[12] = GetBattlerSpriteSubpriority(target) - 2;
			++task->data[0];
			break;
		case 1:
			if (++task->data[1] > 1)
			{
				task->data[1] = 0;
				if (CreateShockWaveLightning(task, taskId))
					++task->data[0];
			}
			break;
		case 2:
			if (task->data[10] == 0)
				DestroyAnimVisualTask(taskId);
			break;
	}
}

//Creates the mini Leech Life Needle
void SpriteCB_LeechLifeNeedle(struct Sprite *sprite)
{
	if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
	{
		gBattleAnimArgs[1] = -gBattleAnimArgs[1];
		gBattleAnimArgs[0] = -gBattleAnimArgs[0];
		StartSpriteAffineAnim(sprite, 1);
	}

	sprite->pos1.x = GetBattlerSpriteCoord2(gBattleAnimTarget, 2) + gBattleAnimArgs[0];
	sprite->pos1.y = GetBattlerSpriteCoord2(gBattleAnimTarget, 3) + gBattleAnimArgs[1];
	sprite->data[0] = gBattleAnimArgs[2];

	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, 2);
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, 3);

	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

//Targeted fire in Doubles for Incinerate

//Launches a projectile from the attacker to the target's centre.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: target x pixel offset
//arg 3: target y pixel offset
//arg 4: duration
//arg 5: wave amplitude
//arg 6: target
void SpriteCB_IncinerateBall(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, 1);

	if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	if (IS_DOUBLE_BATTLE)
	{
		sprite->data[2] = (GetBattlerSpriteCoord2(gBattleAnimTarget, BATTLER_COORD_X_2)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimTarget), BATTLER_COORD_X_2)) / 2;
		sprite->data[4] = (GetBattlerSpriteCoord2(gBattleAnimTarget, BATTLER_COORD_Y)
					   +  GetBattlerSpriteCoord2(PARTNER(gBattleAnimTarget), BATTLER_COORD_Y_PIC_OFFSET)) / 2;
	}
	else
	{
		sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
		sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
	}

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[5] = gBattleAnimArgs[5];
	InitAnimArcTranslation(sprite);

	sprite->callback = SpriteCB_AnimMissileArcStep;
	sprite->invisible = TRUE;
}

//Creates fire that moves diagonally down the opponent
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: target x pixel offset
//arg 3: target y pixel offset
//arg 4: duration
//arg 5: bank
//arg 6: respect pic coords
void SpriteCB_IncinerateFlare(struct Sprite *sprite)
{
    bool8 respectPicCoords;
    u8 coordType;
	u8 target = LoadBattleAnimTarget(5);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		if (SIDE(gBattleAnimAttacker) == SIDE(gBattleAnimTarget)
		&& (gBattleAnimAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
		 || gBattleAnimAttacker == GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)))
		{
			gBattleAnimArgs[2] = -gBattleAnimArgs[2];
		}

		if (!gBattleAnimArgs[6])
		{
			respectPicCoords = TRUE;
			coordType = BATTLER_COORD_Y_PIC_OFFSET;
		}
		else
		{
			respectPicCoords = FALSE;
			coordType = BATTLER_COORD_Y;
		}
		
		if (respectPicCoords)
			InitSpritePosToGivenTargetRespectPicOffsets(sprite, target);
		else
			InitSpritePosToGivenTarget(sprite, target);

		if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
			gBattleAnimArgs[2] = -gBattleAnimArgs[2];

		sprite->data[0] = gBattleAnimArgs[4];
		sprite->data[2] = GetBattlerSpriteCoord(target, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
		sprite->data[4] = GetBattlerSpriteCoord(target, coordType) + gBattleAnimArgs[3];
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}

//Same as burn chance anim but can be used on multiple targets
void SpriteCB_TargetedFireSpread(struct Sprite *sprite)
{
	u8 target = LoadBattleAnimTarget(5);
	
	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		InitSpritePosToGivenTarget(sprite, target);

		sprite->pos2.y = 0; //Reset from being set in above command
		sprite->pos1.y += gBattleAnimArgs[1];
		sprite->data[0] = gBattleAnimArgs[4];
		sprite->data[1] = gBattleAnimArgs[2];
		sprite->data[2] = gBattleAnimArgs[3];

		sprite->callback = TranslateSpriteLinearFixedPoint;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}

//Launches the Dreepy Missile
//arg 0: Target X-Pos
//arg 1: Target Y-Pos
//arg 2: Duration
void SpriteCB_DragonDart(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, TRUE);
	if (SIDE(gBattleAnimTarget) == B_SIDE_OPPONENT)
		StartSpriteAnim(sprite, 1);

	sprite->data[0] = gBattleAnimArgs[2]; //Speed delay
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[0];
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[1];
	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

//Creates The Extreme Evoboost Circles
void SpriteCB_ExtremeEvoboostCircle(struct Sprite *sprite)
{
	InitSpritePosToAnimAttacker(sprite, FALSE);
	sprite->pos1.y += 20;
	sprite->data[1] = 191;
	sprite->callback = (void*) (0x80B190C | 1);
	sprite->callback(sprite);
}

//Creates a twinkle in the upper corner of the screen
void SpriteCB_SparkleInCorner(struct Sprite *sprite)
{
	if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
	{
		sprite->pos1.x = 15;
	}
	else //Enemy side
	{
		sprite->pos1.x = 225;
	}

	sprite->pos1.y = 13;
	sprite->callback = RunStoredCallbackWhenAnimEnds;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

//Creates a twinkle above the target
void SpriteCB_TwinkleAboveTarget(struct Sprite *sprite)
{
	InitSpritePosToAnimTarget(sprite, TRUE);
	sprite->pos1.y = 6;
	sprite->pos2.y = 0;
	sprite->callback = RunStoredCallbackWhenAnimEnds;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

//Creates a twinkle at the location the target was knocked too in Twinkle Tackle
void SpriteCB_TwinkleOnBank(struct Sprite *sprite)
{
	struct Sprite* monSprite;

	if (gBattleAnimArgs[2] == ANIM_TARGET)
		monSprite = &gSprites[GetAnimBattlerSpriteId(ANIM_TARGET)];
	else
		monSprite = &gSprites[GetAnimBattlerSpriteId(ANIM_ATTACKER)];

	sprite->pos1 = monSprite->pos1;
	sprite->pos2 = monSprite->pos2;
	sprite->callback = RunStoredCallbackWhenAnimEnds;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

void SpriteCB_MaxSteelspike(struct Sprite* sprite)
{
	if (--sprite->data[7] == 0)
	{
		FreeSpriteOamMatrix(sprite);
		DestroySprite(sprite);
	}
}

static const union AffineAnimCmd sSpriteAffineAnim_MaxFlareBall[] =
{
	AFFINEANIMCMD_FRAME(4, 4, 0, 32), //1.5x Size
	//Fluctuate Size
	AFFINEANIMCMD_FRAME(-4, -4, 0, 32), //Normal size
	AFFINEANIMCMD_FRAME(0, 0, 0, 8), //Pause
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_MaxFlareBalLooped[] =
{
	AFFINEANIMCMD_FRAME(8, 8, 0, 32), //Get to double size
	AFFINEANIMCMD_FRAME(-4, -4, 0, 8),
	AFFINEANIMCMD_FRAME(4, 4, 0, 8),
	AFFINEANIMCMD_JUMP(2),
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_MaxFlareBall[] =
{
	sSpriteAffineAnim_MaxFlareBall,
	sSpriteAffineAnim_MaxFlareBalLooped,
};

//Creates a fireball on the attacker for Max Flare
//arg 0: X pixel offset
//arg 1: Y pixel offset
void SpriteCB_MaxFlareBallOnAttacker(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, TRUE);
	StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
	sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
}

static void SpriteCB_MaxFlareBallStep(struct Sprite *sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite))
	{
		DestroyAnimSprite(sprite);
	}
	else
	{
		s16 tempData[8];
		s16* data = sprite->data;
		u16 x1 = sprite->pos1.x;
		s16 x2 = sprite->pos2.x;
		u16 y1 = sprite->pos1.y;
		s16 y2 = sprite->pos2.y;
		int i;

		for (i = 0; i < 8; i++)
			tempData[i] = data[i];

		x2 += x1;
		y2 += y1;

		if (!TranslateAnimHorizontalArc(sprite))
		{
			for (i = 0; i < 8; i++)
				data[i] = tempData[i];
		}
	}
}

const struct OamData sGeyserOam =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1, //Above sprites
};

//Launches the fireball for Max Flare
//arg 0: X pixel offset
//arg 1: Y pixel offset
//arg 2: Duration
//arg 3: Wave Amplitude
void SpriteCB_MaxFlareBall(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, TRUE);
	StartSpriteAffineAnim(sprite, 1);

	sprite->data[0] = gBattleAnimArgs[2];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2); //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET); //Target Y
	sprite->data[5] = gBattleAnimArgs[3];

	InitAnimArcTranslation(sprite);
	sprite->callback = SpriteCB_MaxFlareBallStep;
}

void SpriteCB_StonePillarStep2(struct Sprite* sprite)
{
	if (!sprite->affineAnimEnded)
	{
		//Counteract the rotation movement
		if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
			sprite->pos2.x--;
		else
			sprite->pos2.x++;

		switch (sprite->data[1]++) {
			case 8:
			case 11:
			case 13:
			case 16:
			case 19:
			case 22:
			case 25 ... 70:
				sprite->pos2.y++; //Counteract the rotation lifting
		}
	}
	else if (sprite->data[0]++ == 0x25) //Wait 0x25 frames and then disappear
		sprite->callback = DestroySpriteAndMatrix;
}

void SpriteCB_StonePillarStep1(struct Sprite* sprite)
{
	if (sprite->data[0]++ == 0x48)
	{
		if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
			StartSpriteAffineAnim(sprite, 1);
		else
			StartSpriteAffineAnim(sprite, 2);

		sprite->data[0] = 0; sprite->data[1] = 0;
		sprite->callback = SpriteCB_StonePillarStep2;
	}
}

//Creates a stone pillar that falls on the target
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
void SpriteCB_StonePillar(struct Sprite* sprite)
{
	InitSpritePosToAnimTarget(sprite, TRUE);

	sprite->pos2.x = gBattleAnimArgs[0];
	if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
	{
		sprite->pos2.x *= -1;
		sprite->pos2.y *= -1;
	}

	sprite->callback = SpriteCB_StonePillarStep1;
}

//Launches an object upwards
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
void SpriteCB_LaunchObjectUpwards(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, FALSE);
	sprite->data[0] = gBattleAnimArgs[0] > 0 ? 1 : -1;
	sprite->callback = (void*) (0x80B8DE8 | 1); //AnimMudSportDirtRising
}

//Launches an object upwards like they were being shot from a geyser
//arg 0: target
//arg 1: initial x pixel offset
//arg 2: initial y pixel offset
void SpriteCB_Geyser(struct Sprite* sprite)
{
	u8 target = LoadBattleAnimTarget(0);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		sprite->pos1.x = GetBattlerSpriteCoord(target, BATTLER_COORD_X_2) + gBattleAnimArgs[1];
		sprite->pos1.y = GetBattlerSpriteCoord(target, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[2];
	}

	sprite->data[0] = gBattleAnimArgs[1] > 0 ? 1 : -1;
	sprite->callback = (void*) (0x80B8DE8 | 1); //AnimMudSportDirtRising
}

//Animates the beam of light
void SpriteCB_BeamUpStep(struct Sprite* sprite)
{
	if (sprite->data[1]-- == 0)
		StartSpriteAffineAnim(sprite, 1);

	if (sprite->data[0]-- <= 0)
		DestroySpriteAndMatrix(sprite);
}

//Imitates launching a beam of light towards the sky
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: bank
//arg 3: duration
//arg 4: Anim num
//arg 5: affine anim start delay
void SpriteCB_StarfallBeam(struct Sprite* sprite)
{
	u8 target = LoadBattleAnimTarget(2);

	if (!IsBattlerSpriteVisible(target))
		DestroyAnimSprite(sprite);
	else
	{
		InitSpritePosToGivenTarget(sprite, target);
		StartSpriteAnim(sprite, gBattleAnimArgs[4]);
		sprite->data[0] = gBattleAnimArgs[3];
		sprite->data[1] = gBattleAnimArgs[5];
		sprite->callback = SpriteCB_BeamUpStep;
	}
}

//The objects count down, then converge on the target
void SpriteCB_MaxPhantasmObjectStep3(struct Sprite* sprite)
{
	if (sprite->data[7]++ >= 0x10) //Wait quarter of a second
	{
		PlaySE(0xCF); //Hard bam
		sprite->pos1.x += sprite->pos2.x; //Combine positions
		sprite->pos1.y += sprite->pos2.y;
		sprite->pos2.x = 0; //Reset offsets
		sprite->pos2.y = 0;

		sprite->data[0] = 0x8; //Speed delay
		sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2); //Converge on target
		sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	}
}

//The objects move towards the target in an arg
void SpriteCB_MaxPhantasmObjectStep2(struct Sprite* sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite))
	{
		sprite->data[7] = 0;
		sprite->callback = SpriteCB_MaxPhantasmObjectStep3;
	}
}

//The objects prepare to move towards the target
void SpriteCB_MaxPhantasmObjectStep1(struct Sprite* sprite)
{
	if (!FuncIsActiveTask(AnimTask_DynamaxGrowthStep))
	{
		PlaySE(0x98); //Shadow Ball hit
		InitAnimArcTranslation(sprite);
		sprite->data[7] = 0;
		sprite->callback = SpriteCB_MaxPhantasmObjectStep2;
	}
}

//Moves the objects created in Max Phantasm
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: dest x pixel offset
//arg 3: dest y pixel offset
//arg 4: wave amplitude
void SpriteCB_MaxPhantasmObject(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, FALSE);

	if (SIDE(gBattleAnimAttacker))
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	sprite->data[0] = 0x20; //Speed delay
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[4]; //Wave amplitude

	sprite->callback = SpriteCB_MaxPhantasmObjectStep1;
}

//Destroys the butterflies when they reach the target
void SpriteCB_MaxFlutterbyStep2(struct Sprite* sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite))
		DestroySpriteAndMatrix(sprite);
}

//The butterflies prepare to move towards the target
void SpriteCB_MaxFlutterbyStep1(struct Sprite* sprite)
{
	if (!FuncIsActiveTask(AnimTask_DynamaxGrowthStep))
	{
		PlaySE(0x98); //Shadow Ball hit
		StartSpriteAffineAnim(sprite, 1);
		InitAnimArcTranslation(sprite);
		sprite->callback = SpriteCB_MaxFlutterbyStep2;
	}
}

//Moves the butterflies created in Max Flutterby
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: wave amplitude
void SpriteCB_MaxFlutterby(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, FALSE);

	sprite->data[0] = 0x10; //Speed delay
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2); //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET); //Target Y
	sprite->data[5] = gBattleAnimArgs[2]; //Wave amplitude

	sprite->callback = SpriteCB_MaxFlutterbyStep1;
}

void SpriteCB_MaxWyrmwindTornadoStep3(struct Sprite* sprite)
{
	if (sprite->data[6]++ >= sprite->data[7])
		DestroySpriteAndMatrix(sprite);
}

void SpriteCB_MaxWyrmwindTornadoStep2(struct Sprite* sprite)
{
	sprite->data[6] = 0;
	sprite->data[7] = gBattleAnimArgs[5]; //Existance time on target - shouldn't be overwritten prior
	sprite->callback = SpriteCB_MaxWyrmwindTornadoStep3;
}

void SpriteCB_MaxWyrmwindTornadoStep1(struct Sprite* sprite)
{
	if (!FuncIsActiveTask(AnimTask_DynamaxGrowthStep))
	{
		sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, SpriteCB_MaxWyrmwindTornadoStep2);
	}

	if (!sprite->affineAnimEnded && (sprite->data[6]++ & 1) == 0)
		sprite->pos1.y--; //Counteract growth
}

//Creates a Tornado on the attacker and launches it at the target
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: dest x pixel offset
//arg 3: dest y pixel offset
//arg 4: move speed
//arg 5: existance time on target
void SpriteCB_MaxWyrmwindTornado(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, FALSE);

	sprite->data[0] = gBattleAnimArgs[4]; //Speed delay
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2];
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];

	sprite->callback = SpriteCB_MaxWyrmwindTornadoStep1;
}


void SpriteCB_RolloutExplosion(struct Sprite *sprite)
{
	if (TranslateAnimHorizontalArc(sprite))
	{
		//u8 taskId = FindTaskIdByFunc((void*) 0x80B4D01); //AnimTask_RolloutRocksStep
		//if (taskId != 0xFF && gTasks[taskId].data[11] > 0)
		//	gTasks[taskId].data[11]--;

		DestroySprite(sprite);
	}
}

//Creates vines to whip the target for G-Max Vine Lash
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: flip horizontal
void SpriteCB_VineLashWhip(struct Sprite* sprite)
{
	if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
		gBattleAnimArgs[0] *= -1;

	StartSpriteAffineAnim(sprite, gBattleAnimArgs[2]);
	InitSpritePosToAnimTarget(sprite, TRUE);
	StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
	sprite->callback = RunStoredCallbackWhenAnimEnds;
}

static void SpriteCB_CustomArc_Step(struct Sprite *sprite)
{
	sprite->invisible = FALSE;

	if (TranslateAnimHorizontalArc(sprite))
		DestroyAnimSprite(sprite);
}

//Launches a projectile from the attacker's cannons to the target.
//arg 0: initial x pixel offset
//arg 1: initial y pixel offset
//arg 2: target x pixel offset
//arg 3: target y pixel offset
//arg 4: duration
//arg 5: wave amplitude
void SpriteCB_CannonadeBall(struct Sprite* sprite)
{
	InitSpritePosToAnimAttacker(sprite, 0);

	if (SIDE(gBattleAnimAttacker))
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];

	sprite->data[0] = gBattleAnimArgs[4];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + gBattleAnimArgs[2]; //Target X
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3]; //Target Y
	sprite->data[5] = gBattleAnimArgs[5];
	InitAnimArcTranslation(sprite);

	sprite->callback = SpriteCB_CustomArc_Step;
}

//Anim Tasks//
static const union AffineAnimCmd sSpriteAffineAnim_HyperspaceFuryEnemySide[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 32, 1), //Rotate 45 degrees left
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_HyperspaceFuryPlayerSide[] =
{
	AFFINEANIMCMD_FRAME(0, 0, -32, 1), //Rotate 45 degrees right
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd* const sSpriteAffineAnim_HyperspaceFuryMon[] =
{
	sSpriteAffineAnim_HyperspaceFuryEnemySide,
	sSpriteAffineAnim_HyperspaceFuryPlayerSide,
};

//Creates a sideways sprite of the attacker behind the enemy's side
void AnimTask_CreateHyperspaceFuryMon(u8 taskId)
{
	u8 spriteId, subpriority, isBackPic;
	u16 species;
	s16 x;
	u32 personality, otId;
	struct Pokemon* monAtk;

	switch (gTasks[taskId].data[0]) {
		case 0: //Set up sprite
			monAtk = GetIllusionPartyData(gBattleAnimAttacker);
			personality = GetMonData(monAtk, MON_DATA_PERSONALITY, NULL);
			otId = GetMonData(monAtk, MON_DATA_OT_ID, NULL);
			if (gBattleSpritesDataPtr->bankData[gBattleAnimAttacker].transformSpecies == SPECIES_NONE)
				species = GetMonData(monAtk, MON_DATA_SPECIES, NULL);
			else
				species = gBattleSpritesDataPtr->bankData[gBattleAnimAttacker].transformSpecies;

			if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
			{
				
				subpriority = gSprites[GetAnimBattlerSpriteId(ANIM_TARGET)].subpriority + 1;
				isBackPic = 0;
				x = 272;
			}
			else
			{
				subpriority = gSprites[GetAnimBattlerSpriteId(ANIM_TARGET)].subpriority - 1;
				isBackPic = 1;
				x = -32;
			}

			spriteId = CreateMonPicBattleAnim(species, isBackPic, 0, x, GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y), subpriority, personality, otId, gBattleAnimAttacker, 0);
			gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_NORMAL;
			gSprites[spriteId].affineAnims = sSpriteAffineAnim_HyperspaceFuryMon;
			CalcCenterToCornerVec(&gSprites[spriteId], gSprites[spriteId].oam.shape, gSprites[spriteId].oam.size, gSprites[spriteId].oam.affineMode);
			InitSpriteAffineAnim(&gSprites[spriteId]);
			if (SIDE(gBattleAnimAttacker) == B_SIDE_OPPONENT)
				StartSpriteAffineAnim(&gSprites[spriteId], 1);
			PlayCry1(species, 0);
			gTasks[taskId].data[1] = spriteId;
			gTasks[taskId].data[0]++;
			break;
		case 1: //Hoopa pop in
			spriteId = gTasks[taskId].data[1];
			if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
			{
				if (gSprites[spriteId].pos2.x > -30)
					gSprites[spriteId].pos2.x -= 3;
				else if (!IsCryPlaying())
					gTasks[taskId].data[0]++;
			}
			else
			{
				if (gSprites[spriteId].pos2.x < 30)
					gSprites[spriteId].pos2.x += 3;
				else if (!IsCryPlaying())
					gTasks[taskId].data[0]++;
			}
			break;
		case 2: //Hoopa pop out
			spriteId = gTasks[taskId].data[1];
			if (SIDE(gBattleAnimAttacker) == B_SIDE_PLAYER)
			{
				if (gSprites[spriteId].pos2.x < 0)
					gSprites[spriteId].pos2.x += 4;
				else
					gTasks[taskId].data[0]++;
			}
			else
			{
				if (gSprites[spriteId].pos2.x > 0)
					gSprites[spriteId].pos2.x -= 4;
				else
					gTasks[taskId].data[0]++;
			}
			break;
		case 3: //Destroy sprite
			spriteId = gTasks[taskId].data[1];
			ClearPokemonCrySongs();
			DestroySpriteAndFreeResources(&gSprites[spriteId]);
			DestroyAnimVisualTask(taskId);
			break;
	}
}

extern const struct SpriteTemplate gRolloutExplosionSpriteTemplate;
static void CreateRolloutExplosionSprite(struct Task* task)
{
	u8 spriteId;
	u16 x, y;
	const struct SpriteTemplate* spriteTemplate = &gRolloutExplosionSpriteTemplate;

	x = task->data[2] >> 3;
	y = task->data[3] >> 3;
	x += (task->data[12] * 4);

	spriteId = CreateSprite(spriteTemplate, x, y, 35);
	if (spriteId != MAX_SPRITES)
	{
		gSprites[spriteId].data[0] = 18;
		gSprites[spriteId].data[2] = ((task->data[12] * 20) + x) + (task->data[1] * 3);
		gSprites[spriteId].data[4] = y;
		gSprites[spriteId].data[5] = -16 - (task->data[1] * 2);

		InitAnimArcTranslation(&gSprites[spriteId]);
		task->data[11]++;
	}

	task->data[12] *= -1;
}

static void AnimTask_RolloutPathExplosionsStep(u8 taskId)
{
	struct Task *task;

	task = &gTasks[taskId];

	switch (task->data[0]) {
		case 0:
			if (++task->data[9] == 10)
			{
				task->data[11] = 20;
				task->data[0]++;
			}
			break;
		case 1:
			if (--task->data[11] == 0)
				task->data[0]++;
			break;
		case 2:
			if (--task->data[9] == 0)
			{
				task->data[6] = 0;
				task->data[7] = 0;
				task->data[0]++;
			}
			break;
		case 3:
			task->data[2] += task->data[4];
			task->data[3] += task->data[5];
			if (++task->data[9] >= task->data[10])
			{
				task->data[9] = 0;
				CreateRolloutExplosionSprite(task);
				task->data[13] += task->data[14];
			}

			if (--task->data[8] == 0)
			{
				task->data[0]++;
			}
			break;
		case 4:
			DestroyAnimVisualTask(taskId);
			break;
	}
}

//Creates explosions on the same path as Rollout rocks
//No args
void AnimTask_RolloutPathExplosions(u8 taskId)
{
	int var5;
	u16 var0, var1, var2, var3, var4;
	struct Task *task;

	task = &gTasks[taskId];

	var0 = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
	var1 = GetBattlerSpriteCoord(gBattleAnimAttacker, 1) + 24;
	var2 = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
	var3 = GetBattlerSpriteCoord(gBattleAnimTarget, 1) + 24;

	if (BATTLE_PARTNER(gBattleAnimAttacker) == gBattleAnimTarget)
		var3 = var1;

	var4 = 4;
	task->data[8] = 48 - (var4 * 8);

	task->data[0] = 0;
	task->data[11] = 0;
	task->data[9] = 0;
	task->data[12] = 1;

	var5 = task->data[8];
	if (var5 < 0)
		var5 += 7;

	task->data[10] = (var5 >> 3) - 1;

	task->data[2] = var0 * 8;
	task->data[3] = var1 * 8;
	task->data[4] = ((var2 - var0) * 8) / task->data[8];
	task->data[5] = ((var3 - var1) * 8) / task->data[8];
	task->data[6] = 0;
	task->data[7] = 0;

	task->data[1] = var4;
	task->data[15] = GetAnimBattlerSpriteId(0);

	task->func = AnimTask_RolloutPathExplosionsStep;
}

extern const struct SpriteTemplate gMaxSteelspikeSpriteTemplate;
static void CreateSteelspikeSprite(struct Task* task)
{
	u8 spriteId;
	u16 x, y;

	x = task->data[2] >> 3;
	y = task->data[3] >> 3;
	x += (task->data[12] * 8);

	spriteId = CreateSprite(&gMaxSteelspikeSpriteTemplate, x, y, 35);
	if (spriteId != MAX_SPRITES)
	{
		gSprites[spriteId].data[0] = 18;
		gSprites[spriteId].data[2] = ((task->data[12] * 20) + x) + (task->data[1] * 3);
		gSprites[spriteId].data[4] = y;
		gSprites[spriteId].data[5] = -16 - (task->data[1] * 2);
		gSprites[spriteId].data[7] = task->data[8] + 60;

		PlaySE(0x71);
		task->data[11]++;
	}

	task->data[12] *= -1;
}

extern const struct SpriteTemplate gMaxSteelspikeCurvedSpriteTemplate;
static void AnimTask_MaxSteelspikeStep(u8 taskId)
{
	struct Task *task;
	u8 spriteId1, spriteId2;
	u16 x, y;

	task = &gTasks[taskId];

	switch (task->data[0]) {
		case 0:
			task->data[2] += task->data[4];
			task->data[3] += task->data[5];
			if (++task->data[9] >= task->data[10])
			{
				task->data[9] = 0;
				CreateSteelspikeSprite(task);
				task->data[13] += task->data[14];
			}

			if (--task->data[8] == 0)
			{
				task->data[11] = 0;
				task->data[0]++;
			}
			break;
		case 1:
			if (++task->data[11] == 10)
				task->data[0]++;
			break;
		case 2: ;
			x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
			y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y);

			spriteId1 = CreateSprite(&gMaxSteelspikeCurvedSpriteTemplate, x + 5, y, 35);
			spriteId2 = CreateSprite(&gMaxSteelspikeCurvedSpriteTemplate, x - 5, y, 35);
			StartSpriteAffineAnim(&gSprites[spriteId1], 0); //Point Left
			gSprites[spriteId1].data[7] = 50;
			StartSpriteAffineAnim(&gSprites[spriteId2], 1); //Point Right
			gSprites[spriteId2].data[7] = 50;

			if (SIDE(gBattleAnimTarget) == B_SIDE_PLAYER)
			{
				gSprites[spriteId1].oam.priority = 1; //Above player's Pokemon
				gSprites[spriteId2].oam.priority = 1; //Above player's Pokemon
				gSprites[spriteId1].pos1.y += 15;
				gSprites[spriteId2].pos2.y += 15;
			}

			task->data[0]++;
			task->data[11] = 0;
			break;
		case 3:
			if (++task->data[11] == 7)
				task->data[0]++;
			break;
		case 4:
			DestroyAnimVisualTask(taskId);
	}
}

//Creates explosions on the same path as Rollout rocks
//No args
void AnimTask_MaxSteelspike(u8 taskId)
{
	int var5;
	u16 var0, var1, var2, var3, var4;
	struct Task *task;

	task = &gTasks[taskId];

	var0 = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
	var1 = GetBattlerSpriteCoord(gBattleAnimAttacker, 1) + 24;
	var2 = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
	var3 = GetBattlerSpriteCoord(gBattleAnimTarget, 1) + 24;

	if (BATTLE_PARTNER(gBattleAnimAttacker) == gBattleAnimTarget)
		var3 = var1;

	var4 = 4;
	task->data[8] = 48 - (var4 * 8);

	task->data[0] = 0;
	task->data[11] = 0;
	task->data[9] = 0;
	task->data[12] = 1;

	var5 = task->data[8];
	if (var5 < 0)
		var5 += 7;

	task->data[10] = (var5 >> 3) - 1;

	task->data[2] = var0 * 8;
	task->data[3] = var1 * 8;
	task->data[4] = ((var2 - var0) * 8) / task->data[8];
	task->data[5] = ((var3 - var1) * 8) / task->data[8];
	task->data[6] = 0;
	task->data[7] = 0;

	task->data[1] = var4;
	task->data[15] = GetAnimBattlerSpriteId(0);

	task->func = AnimTask_MaxSteelspikeStep;
}

//Creates purple flames that surround the target.
//No args.
void AnimTask_PurpleFlamesOnTarget(u8 taskId)
{
	struct Task *task = &gTasks[taskId];

	task->data[0] = 0;
	task->data[1] = 16;
	task->data[9] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
	task->data[10] = GetBattlerYCoordWithElevation(gBattleAnimTarget);
	task->data[11] = (GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_WIDTH) / 2) + 8;
	task->data[7] = 0;
	task->data[5] = GetBattlerSpriteBGPriority(gBattleAnimTarget);
	task->data[6] = GetBattlerSpriteSubpriority(gBattleAnimTarget) - 2;
	task->data[3] = 0;
	task->data[4] = 16;
	SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL));
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0x10));
	task->data[8] = 0;
	task->func = (void*) (0x80B696C | 1);
}

//Converts the given palette tag to grayscale
void AnimTask_GrayscaleParticle(u8 taskId)
{
	u16 palTag = gBattleAnimArgs[0];
	TintPalette_GrayScale(&gPlttBufferUnfaded2[IndexOfSpritePaletteTag(palTag) * 16], 16);
	TintPalette_GrayScale(&gPlttBufferFaded2[IndexOfSpritePaletteTag(palTag) * 16], 16);
	TintPalette_GrayScale(&((u16*) PLTT)[IndexOfSpritePaletteTag(palTag) * 16], 16);
	DestroyAnimVisualTask(taskId);
}

// Scales up the target mon sprite
// Used in Let's Snuggle Forever
// No args.
void AnimTask_GrowTarget(u8 taskId)
{
	u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
	PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_BLEND);
	SetSpriteRotScale(spriteId, 208, 208, 0);
	gTasks[taskId].data[0] = 120;
	gTasks[taskId].func = AnimTask_GrowStep;
}

static void AnimTask_GrowStep(u8 taskId)
{
	if (--gTasks[taskId].data[0] == -1)
	{
		u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
		ResetSpriteRotScale(spriteId);
		DestroyAnimVisualTask(taskId);
	}
}

static const union AffineAnimCmd sDynamaxGrowthAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(-2, -2, 0, 64), //Double in size over 1 second
	AFFINEANIMCMD_FRAME(0, 0, 0, 64), //Pause for 1 seconds
	AFFINEANIMCMD_FRAME(16, 16, 0, 8), //Shrink back down in 1/8 of a second
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sDynamaxGrowthAttackAnimationAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(-4, -4, 0, 32), //Double in size quicker
	AFFINEANIMCMD_FRAME(0, 0, 0, 32), //Pause for less
	AFFINEANIMCMD_FRAME(16, 16, 0, 8),
	AFFINEANIMCMD_END,
};

static void AnimTask_DynamaxGrowthStep(u8 taskId)
{
	struct Task* task = &gTasks[taskId];
	if (!RunAffineAnimFromTaskData(task))
		DestroyAnimVisualTask(taskId);
}

//Arg 0: Animation for attack
void AnimTask_DynamaxGrowth(u8 taskId)
{
	struct Task* task = &gTasks[taskId];
	u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);

	if (gBattleAnimArgs[0] == 0)
		PrepareAffineAnimInTaskData(task, spriteId, sDynamaxGrowthAffineAnimCmds);
	else
		PrepareAffineAnimInTaskData(task, spriteId, sDynamaxGrowthAttackAnimationAffineAnimCmds);
	task->func = AnimTask_DynamaxGrowthStep;
}

static const union AffineAnimCmd sSpriteAffineAnim_GrowingFist[] =
{
	AFFINEANIMCMD_FRAME(4, 4, 0, 64), //Double in size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gSpriteAffineAnimTable_GrowingFist[] =
{
	sSpriteAffineAnim_GrowingFist,
};

static const union AffineAnimCmd sSquishTargetAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(0, 64, 0, 16), //Flatten
	AFFINEANIMCMD_FRAME(0, 0, 0, 64),
	AFFINEANIMCMD_FRAME(0, -64, 0, 16),
	AFFINEANIMCMD_END,
};

void AnimTask_SquishTarget(u8 taskId)
{
	struct Task* task = &gTasks[taskId];
	u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
	PrepareAffineAnimInTaskData(task, spriteId, sSquishTargetAffineAnimCmds);
	task->func = AnimTask_DynamaxGrowthStep;
}

static const union AffineAnimCmd sCompressTargetHorizontallyAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(64, 0, 0, 16), //Compress
	AFFINEANIMCMD_FRAME(0, 0, 0, 64),
	AFFINEANIMCMD_FRAME(-64, 0, 0, 16),
	AFFINEANIMCMD_END,
};

void AnimTask_CompressTargetHorizontally(u8 taskId)
{
	struct Task* task = &gTasks[taskId];
	u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
	PrepareAffineAnimInTaskData(task, spriteId, sCompressTargetHorizontallyAffineAnimCmds);
	task->func = AnimTask_DynamaxGrowthStep;
}

#define tSpriteId data[0]
#define tTimer data[1]
#define tInitialXPos data[2]
#define tInitialYPos data[3]
#define tSide data[4]
#define tAnimLengthTime data[5]
static const s8 sHomerunEnemyHorizontalMovement[] =
{
	3, 3, 3, 3,
	3, 3, 2, 2,
	1, 1, 1, 1,
	1, 1, 1, 1,
	0, 1, 0, 1,
	0, 1, 0, 0,
	1, 0, 0, 1,
	0, 0, 0, 1,
	0, 0, 0, 1,
};

static const s8 sHomerunEnemyVerticalMovement[] =
{
	-4, -4, -4, -4,
	-4, -3, -3, -2,
	-2, -1, -1, -1,
	-1, -1, -1, -1,
	 0, -1,  0, -1,
	 0, -1,  0,  0,
	 0,  0, -1,  0,
	 0, -1,  0,  0,
	-1,  0,  0,  0,
};

void AnimTask_TwinkleTackleLaunchStep(u8 taskId)
{
	u16 rotation;
	s16 xScale, yScale;
	struct Task* task = &gTasks[taskId];
	struct Sprite* sprite = &gSprites[task->tSpriteId];

	if (task->tTimer > task->tAnimLengthTime)
	{
		if (task->tTimer > task->tAnimLengthTime + 5) //Wait an extra few frames so the glint can be placed on the target
		{
			sprite->pos1.x = task->tInitialXPos;
			sprite->pos1.y = task->tInitialYPos;
			ResetSpriteRotScale(task->tSpriteId);
			DestroyAnimVisualTask(taskId);
		}
		else
			++task->tTimer;
		return;
	}
	else if ((u16) task->tTimer < NELEMS(sHomerunEnemyHorizontalMovement))
	{
		s8 movement = sHomerunEnemyHorizontalMovement[task->tTimer];
		if (task->tSide == B_SIDE_PLAYER)
			movement *= -1;
		sprite->pos1.x += movement;

		movement = sHomerunEnemyVerticalMovement[task->tTimer];
		if (task->tSide == B_SIDE_PLAYER)
			movement *= -1;
		sprite->pos1.y += movement;
	}

	xScale = 0x180;
	yScale = 0x180;
	rotation = (task->tTimer << 4) + (task->tTimer << 3);

	xScale += rotation;
	yScale += rotation;
	rotation <<= 7;

	if (task->tSide == B_SIDE_OPPONENT)
		rotation *= -1;

	SetSpriteRotScale(task->tSpriteId, xScale, yScale, rotation);

	if (++task->tTimer > task->tAnimLengthTime)
		sprite->invisible = TRUE;
}

//Launches the target in Twinkle Tackle
//arg 0: Anim time
void AnimTask_TwinkleTackleLaunch(u8 taskId)
{
	struct Task* task = &gTasks[taskId];

	task->tSpriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
	task->tSide = SIDE(gBattleAnimTarget);
	task->tAnimLengthTime = gBattleAnimArgs[0];
	task->tInitialXPos = gSprites[task->tSpriteId].pos1.x;
	task->tInitialYPos = gSprites[task->tSpriteId].pos1.y;
	task->tTimer = 0;
	task->func = AnimTask_TwinkleTackleLaunchStep;

	PrepareBattlerSpriteForRotScale(task->tSpriteId, ST_OAM_OBJ_NORMAL);
}
#undef tSpriteId
#undef tTimer
#undef tInitialXPos
#undef tInitialYPos
#undef tSide
#undef tAnimLengthTime

void AnimTask_AllBanksInvisible(u8 taskId)
{
	for (int i = 0; i < gBattlersCount; ++i)
	{
		u8 spriteId = gBattlerSpriteIds[i];

		if (spriteId == 0xFF || !IsBattlerSpriteVisible(i)) //Pokemon that are already hidden
			gNewBS->hiddenAnimBattlerSprites |= gBitTable[i]; //Set bit to keep hidden after animation
		else
			gSprites[spriteId].invisible = TRUE;
	}

	DestroyAnimVisualTask(taskId);
}

void AnimTask_AllBanksVisible(u8 taskId)
{
	for (int i = 0; i < gBattlersCount; ++i)
	{
		u8 spriteId = gBattlerSpriteIds[i];

		if (spriteId == 0xFF || gNewBS->hiddenAnimBattlerSprites & gBitTable[i]) //Pokemon that are already hidden
			gNewBS->hiddenAnimBattlerSprites &= ~gBitTable[i]; //Clear bit to keep hidden after animation
		else
			gSprites[spriteId].invisible = FALSE;
	}

	DestroyAnimVisualTask(taskId);
}

void AnimTask_AllBanksInvisibleExceptAttackerAndTarget(u8 taskId)
{
	for (int i = 0; i < gBattlersCount; ++i)
	{
		u8 spriteId = gBattlerSpriteIds[i];

		if (spriteId == GetAnimBattlerSpriteId(ANIM_BANK_ATTACKER)
		||  spriteId == GetAnimBattlerSpriteId(ANIM_BANK_TARGET))
			continue;

		if (spriteId == 0xFF || !IsBattlerSpriteVisible(i)) //Pokemon that are already hidden
			gNewBS->hiddenAnimBattlerSprites |= gBitTable[i]; //Set bit to keep hidden after animation
		else
			gSprites[spriteId].invisible = TRUE;
	}

	DestroyAnimVisualTask(taskId);
}

#define RESTORE_HIDDEN_HEALTHBOXES												\
{																				\
	if (priority == 0)															\
		Memset(gNewBS->hiddenHealthboxFlags, 0, sizeof(gNewBS->hiddenHealthboxFlags)); \
	for (spriteId = 0; spriteId < MAX_SPRITES; ++spriteId)						\
	{																			\
		switch (gSprites[spriteId].template->tileTag) {							\
			case TAG_HEALTHBOX_PLAYER1_TILE:									\
			case TAG_HEALTHBOX_PLAYER2_TILE:									\
			case TAG_HEALTHBOX_OPPONENT1_TILE:									\
			case TAG_HEALTHBOX_OPPONENT2_TILE:									\
			case TAG_HEALTHBAR_PLAYER1_TILE:									\
			case TAG_HEALTHBAR_OPPONENT1_TILE:									\
			case TAG_HEALTHBAR_PLAYER2_TILE:									\
			case TAG_HEALTHBAR_OPPONENT2_TILE:									\
				switch (priority) {												\
					case 0:														\
						ChangeHealthboxVisibility(spriteId, TRUE);				\
						break;													\
					default:													\
						if (IsRaidBattle()										\
						&& GetBankPartyData(BANK_RAID_BOSS)->hp == 0			\
						&& (gSprites[spriteId].template->tileTag == TAG_HEALTHBOX_OPPONENT1_TILE || gSprites[spriteId].template->tileTag == TAG_HEALTHBAR_OPPONENT1_TILE)) \
						{														\
							gNewBS->hiddenHealthboxFlags[spriteId / 8] &= ~gBitTable[spriteId % 8]; \
							continue;											\
						}														\
																				\
						ChangeHealthboxVisibility(spriteId, FALSE);				\
				}																\
		}																		\
	}																			\
}

static void ChangeHealthboxVisibility(u8 spriteId, bool8 hide)
{
	struct Sprite* sprite = &gSprites[spriteId];

	if (gNewBS == NULL) //Battle struct was already freed at end of battle
		return;

	if (hide)
	{
		if (!sprite->invisible) //Sprite isn't already hidden
		{
			sprite->invisible = TRUE;
			gNewBS->hiddenHealthboxFlags[spriteId / 8] |= gBitTable[spriteId % 8]; //Set special hidden flag
		}
	}
	else
	{
		if (sprite->invisible && gNewBS->hiddenHealthboxFlags[spriteId / 8] & gBitTable[spriteId % 8]) //Sprite was hidden during animation
		{
			sprite->invisible = FALSE;
			gNewBS->hiddenHealthboxFlags[spriteId / 8] &= ~gBitTable[spriteId % 8]; //Remove special hidden flag
		}
	}
}

#define hMain_HealthBarSpriteId	 data[5]
void UpdateOamPriorityInAllHealthboxes(u8 priority)
{
	u32 i, spriteId;

	#ifndef HIDE_HEALTHBOXES_DURING_ANIMS
		goto DEFAULT_CASE;
	#endif

	switch (gBattleBufferA[gBattleAnimAttacker][0]) {
		case CONTROLLER_MOVEANIMATION: ;
			if (sAnimMoveIndex == MOVE_TRANSFORM)
				goto DEFAULT_CASE;
			#ifdef DONT_HIDE_HEALTHBOXES_ATTACKER_STATUS_MOVES
			if (gBattleMoves[sAnimMoveIndex].target & MOVE_TARGET_USER)
				goto DEFAULT_CASE;
			#endif
			goto HIDE_BOXES;

		case CONTROLLER_BALLTHROWANIM:
			goto HIDE_BOXES;

		case CONTROLLER_BATTLEANIMATION:
			switch (gBattleBufferA[gBattleAnimAttacker][1]) {
				case B_ANIM_TURN_TRAP:
				case B_ANIM_LEECH_SEED_DRAIN:
				case B_ANIM_MON_HIT:
				case B_ANIM_SNATCH_MOVE:
				case B_ANIM_FUTURE_SIGHT_HIT:
				case B_ANIM_DOOM_DESIRE_HIT:
				case B_ANIM_WISH_HEAL:
				case B_ANIM_MON_SCARED:
				case B_ANIM_GHOST_GET_OUT:
				case B_ANIM_WISHIWASHI_FISH:
				case B_ANIM_ZYGARDE_CELL_SWIRL:
				case B_ANIM_ELECTRIC_SURGE:
				case B_ANIM_GRASSY_SURGE:
				case B_ANIM_MISTY_SURGE:
				case B_ANIM_PSYCHIC_SURGE:
				case B_ANIM_SEA_OF_FIRE:
				case B_ANIM_LUNAR_DANCE_HEAL:
				case B_ANIM_HEALING_WISH_HEAL:
				case B_ANIM_RED_PRIMAL_REVERSION:
				case B_ANIM_BLUE_PRIMAL_REVERSION:
				case B_ANIM_POWDER_EXPLOSION:
				case B_ANIM_BEAK_BLAST_WARM_UP:
				case B_ANIM_SHELL_TRAP_SET:
				case B_ANIM_BERRY_EAT:
				case B_ANIM_ZMOVE_ACTIVATE:
				case B_ANIM_MEGA_EVOLUTION:
				case B_ANIM_ULTRA_BURST:
				case B_ANIM_DYNAMAX_START:
				case B_ANIM_RAID_BATTLE_ENERGY_BURST:
					goto HIDE_BOXES;
			}
		__attribute__ ((fallthrough));
		default:
		DEFAULT_CASE:
			for (i = 0; i < gBattlersCount; i++)
			{
				u8 healthboxLeftSpriteId = gHealthboxSpriteIds[i];
				u8 healthboxRightSpriteId = gSprites[gHealthboxSpriteIds[i]].oam.affineParam;
				u8 healthbarSpriteId = gSprites[gHealthboxSpriteIds[i]].hMain_HealthBarSpriteId;

				gSprites[healthboxLeftSpriteId].oam.priority = priority;
				gSprites[healthboxRightSpriteId].oam.priority = priority;
				gSprites[healthbarSpriteId].oam.priority = priority;

				if (priority > 0) //Restore Hidden Healthboxes
				{
					RESTORE_HIDDEN_HEALTHBOXES;
				}
			}
			return;
	}

HIDE_BOXES:
	RESTORE_HIDDEN_HEALTHBOXES;
}

u8 CalcHealthBarPixelChange(unusedArg u8 bank)
{
	#ifdef FASTER_HEALTHBOX_CHANGE
		u16 maxHP = GetMonData(GetBankPartyData(bank), MON_DATA_MAX_HP, NULL);
		u16 amount = maxHP / 48; //48 pixels on healthbar
		u16 leftover = maxHP % 48;

		if (leftover >= 40) //So health like 95 (worst case) is included in next level up
			amount += 1;

		return amount;
	#else
		return 1;
	#endif
}

//The original function would run through gBattlerSpriteIds until it found a spriteId
//that matched. This would cause issues if there was a Pokemon missing from the field who
//used to have the same spriteId as the current sprite in the animation.
u16 GetBattlerYDeltaFromSpriteId(u8 spriteId)
{
	u16 species;
    struct BattleSpriteInfo* spriteInfo;
	struct Pokemon* mon;
    u8 bank = gSprites[spriteId].data[0];

	mon = GetIllusionPartyData(bank);
	spriteInfo = gBattleSpritesDataPtr->bankData;
	if (!spriteInfo[bank].transformSpecies)
		species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	else
		species = spriteInfo[bank].transformSpecies;

	if (SIDE(bank) == B_SIDE_PLAYER)
		return gMonBackPicCoords[species].y_offset;
	else
		return gMonFrontPicCoords[species].y_offset;

    return MAX_SPRITES;
}

void HandleSpeciesGfxDataChange(u8 bankAtk, u8 bankDef, u8 transformType)
{
	u8 position;
	u16 targetSpecies, paletteOffset;
	u32 personality, otId;
	const u32 *lzPaletteData;

    if (transformType == 0xFF) //Silph Scope
    {
		const void* src;
		void* dst;

		struct Pokemon* monAtk;
        position = GetBattlerPosition(bankAtk);
		monAtk = GetBankPartyData(bankAtk);
        targetSpecies = GetMonData(monAtk, MON_DATA_SPECIES, NULL);
        personality = GetMonData(monAtk, MON_DATA_PERSONALITY, NULL);
        otId = GetMonData(monAtk, MON_DATA_OT_ID, NULL);

        HandleLoadSpecialPokePic_DontHandleDeoxys(&gMonFrontPicTable[targetSpecies], gMonSpritesGfxPtr->sprites[position], targetSpecies, personality);
        src = gMonSpritesGfxPtr->sprites[position];
        dst = (void *)(VRAM + 0x10000 + gSprites[gBattlerSpriteIds[bankAtk]].oam.tileNum * 32);
        DmaCopy32(3, src, dst, 0x800);
        paletteOffset = 0x100 + bankAtk * 16;
        lzPaletteData = GetMonSpritePalFromSpeciesAndPersonality(targetSpecies, otId, personality);
		LZDecompressWram(lzPaletteData, gDecompressionBuffer);
		LoadPalette(gDecompressionBuffer, paletteOffset, 32);
        TryFadeBankPaletteForDynamax(bankAtk, paletteOffset);
		gSprites[gBattlerSpriteIds[bankAtk]].pos1.y = GetBattlerSpriteDefault_Y(bankAtk);
        StartSpriteAnim(&gSprites[gBattlerSpriteIds[bankAtk]], gBattleMonForms[bankAtk]);
        SetMonData(monAtk, MON_DATA_NICKNAME, gSpeciesNames[targetSpecies]);
        UpdateNickInHealthbox(gHealthboxSpriteIds[bankAtk], monAtk);
        TryAddPokeballIconToHealthbox(gHealthboxSpriteIds[bankAtk], 1);
    }
	else if (transformType == 0xFE) //Transforming animation for form change
	{
		const void* src;
		void* dst;
		struct Pokemon* monAtk;
		const struct CompressedSpriteSheet* spriteSheet;

		position = GetBattlerPosition(bankAtk);
		monAtk = GetBankPartyData(bankAtk);
		targetSpecies = GetMonData(monAtk, MON_DATA_SPECIES, NULL);
		otId = GetMonData(monAtk, MON_DATA_OT_ID, NULL);
		personality = GetMonData(monAtk, MON_DATA_PERSONALITY, NULL);

		if (SIDE(bankAtk) == B_SIDE_PLAYER)
			spriteSheet = &gMonBackPicTable[targetSpecies];
		else
			spriteSheet = &gMonFrontPicTable[targetSpecies];

		HandleLoadSpecialPokePic_DontHandleDeoxys(spriteSheet, gMonSpritesGfxPtr->sprites[position], targetSpecies, personality);
		src = gMonSpritesGfxPtr->sprites[position];
		dst = (void *)(VRAM + 0x10000 + gSprites[gBattlerSpriteIds[bankAtk]].oam.tileNum * 32);
		DmaCopy32(3, src, dst, 0x800);
		paletteOffset = 0x100 + bankAtk * 16;
		lzPaletteData = GetMonSpritePalFromSpeciesAndPersonality(targetSpecies, otId, personality);
		LZDecompressWram(lzPaletteData, gDecompressionBuffer);
		LoadPalette(gDecompressionBuffer, paletteOffset, 32);
		TryFadeBankPaletteForDynamax(bankAtk, paletteOffset);
		gSprites[gBattlerSpriteIds[bankAtk]].pos1.y = GetBattlerSpriteDefault_Y(bankAtk);
		StartSpriteAnim(&gSprites[gBattlerSpriteIds[bankAtk]], gBattleMonForms[bankAtk]);
	}
	else if (transformType != 0) //Castform
	{
		StartSpriteAnim(&gSprites[gBattlerSpriteIds[bankAtk]], gBattleSpritesDataPtr->animationData->animArg);
		paletteOffset = 0x100 + bankAtk * 16;
		LoadPalette(gBattleStruct->castformPalette[gBattleSpritesDataPtr->animationData->animArg], paletteOffset, 32);
		gBattleMonForms[bankAtk] = gBattleSpritesDataPtr->animationData->animArg;
		TryFadeBankPaletteForDynamax(bankAtk, paletteOffset);
		gSprites[gBattlerSpriteIds[bankAtk]].pos1.y = GetBattlerSpriteDefault_Y(bankAtk);
	}
	else //Regular attack Transform
	{
		const void* src;
		void* dst;
		struct Pokemon* monDef;
		const struct CompressedSpriteSheet* spriteSheet;

		position = GetBattlerPosition(bankAtk);
		monDef = GetBankPartyData(bankDef);
		targetSpecies = GetMonData(monDef, MON_DATA_SPECIES, NULL);
		targetSpecies = TryFixDynamaxTransformSpecies(bankAtk, targetSpecies);
		otId = GetMonData(GetBankPartyData(bankAtk)/*monDef*/, MON_DATA_OT_ID, NULL); //Must use monAtk, otherwise will be shiny now, and not on screen reload

		if (SIDE(bankAtk) == B_SIDE_PLAYER)
			spriteSheet = &gMonBackPicTable[targetSpecies];
		else
			spriteSheet = &gMonFrontPicTable[targetSpecies];

		HandleLoadSpecialPokePic_DontHandleDeoxys(spriteSheet, gMonSpritesGfxPtr->sprites[position], targetSpecies, gTransformedPersonalities[bankAtk]);
		src = gMonSpritesGfxPtr->sprites[position];
		dst = (void *)(VRAM + 0x10000 + gSprites[gBattlerSpriteIds[bankAtk]].oam.tileNum * 32);
		DmaCopy32(3, src, dst, 0x800);
		paletteOffset = 0x100 + bankAtk * 16;
		lzPaletteData = GetMonSpritePalFromSpeciesAndPersonality(targetSpecies, otId, gTransformedPersonalities[bankAtk]);
		LZDecompressWram(lzPaletteData, gDecompressionBuffer);
		LoadPalette(gDecompressionBuffer, paletteOffset, 32);

		if (targetSpecies == SPECIES_CASTFORM)
		{
			LZDecompressWram(lzPaletteData, gBattleStruct->castformPalette[0]);
			LoadPalette(gBattleStruct->castformPalette[0] + gBattleMonForms[bankDef] * 16, paletteOffset, 32);
		}

		TryFadeBankPaletteForDynamax(bankAtk, paletteOffset);
		gBattleSpritesDataPtr->bankData[bankAtk].transformSpecies = targetSpecies;
		gBattleMonForms[bankAtk] = gBattleMonForms[bankDef];
		gSprites[gBattlerSpriteIds[bankAtk]].pos1.y = GetBattlerSpriteDefault_Y(bankAtk);
		StartSpriteAnim(&gSprites[gBattlerSpriteIds[bankAtk]], gBattleMonForms[bankAtk]);
	}
}

#define tBattlerId 	data[0]
#define tAnimId 	data[1]
#define tArgumentId	data[2]
#define tState 		data[3]

bool8 TryHandleLaunchBattleTableAnimation(u8 activeBattler, u8 bankAtk, u8 bankDef, u8 tableId, u16 argument)
{
	u8 taskId;

	if (tableId == B_ANIM_CASTFORM_CHANGE && (argument & 0x80))
	{
		gBattleMonForms[activeBattler] = (argument & ~(0x80));
		return TRUE;
	}
	if (gBattleSpritesDataPtr->bankData[activeBattler].behindSubstitute
	&& !ShouldAnimBeDoneRegardlessOfSubsitute(tableId)
	&& !ShouldSubstituteRecedeForSpecialBattleAnim(tableId))
	{
		return TRUE;
	}
	if (gBattleSpritesDataPtr->bankData[activeBattler].behindSubstitute
	&& tableId == B_ANIM_SUBSTITUTE_FADE
	&& gSprites[gBattlerSpriteIds[activeBattler]].invisible)
	{
		LoadBattleMonGfxAndAnimate(activeBattler, TRUE, gBattlerSpriteIds[activeBattler]);
		ClearBehindSubstituteBit(activeBattler);
		return TRUE;
	}

	gBattleAnimAttacker = bankAtk;
	gBattleAnimTarget = bankDef;
	gAnimScriptCallback = NULL;
	taskId = CreateTask(Task_HandleSpecialBattleAnimation, 10);
	gTasks[taskId].tBattlerId = activeBattler;
	gTasks[taskId].tAnimId = tableId;
	gTasks[taskId].tArgumentId = argument;
	gBattleSpritesDataPtr->healthBoxesData[gTasks[taskId].tBattlerId].animFromTableActive = TRUE;

	return FALSE;
}

static void Task_HandleSpecialBattleAnimation(u8 taskId)
{
	u8 activeBattler = gTasks[taskId].tBattlerId;
	u8 animId = gTasks[taskId]. tAnimId;

	if (gAnimScriptCallback != NULL)
		gAnimScriptCallback();

	switch (gTasks[taskId].tState) {
		case 0:
			if (gBattleSpritesDataPtr->bankData[activeBattler].behindSubstitute
			&& !gBattleSpritesDataPtr->bankData[activeBattler].substituteOffScreen
			&& ShouldSubstituteRecedeForSpecialBattleAnim(animId))
			{
				//Temporarily revert the Pokemon to its original form so the anim works properly.
				//This change is done individually on both roms so there should be no issue in link
				//battles.
				TrySwapBackupSpeciesWithSpecies(activeBattler, animId);
				gBattleSpritesDataPtr->bankData[activeBattler].substituteOffScreen = TRUE;
				InitAndLaunchSpecialAnimation(activeBattler, activeBattler, activeBattler, B_ANIM_SUBSTITUTE_TO_MON);
			}
			++gTasks[taskId].tState;
			break;

		case 1:
			if (!gBattleSpritesDataPtr->healthBoxesData[activeBattler].specialAnimActive)
			{
				//Now restore the original species
				if (gBattleSpritesDataPtr->bankData[activeBattler].behindSubstitute
				&&  gBattleSpritesDataPtr->bankData[activeBattler].substituteOffScreen
				&& ShouldSubstituteRecedeForSpecialBattleAnim(animId))
					TrySwapBackupSpeciesWithSpecies(activeBattler, animId);

				gBattleSpritesDataPtr->animationData->animArg = gTasks[taskId].tArgumentId;
				LaunchBattleAnimation(gBattleAnims_General, animId, FALSE);
				++gTasks[taskId].tState;
			}
			break;

		case 2:
			if (!gAnimScriptActive)
			{
				if (gBattleSpritesDataPtr->bankData[activeBattler].behindSubstitute
				&&  gBattleSpritesDataPtr->bankData[activeBattler].substituteOffScreen == TRUE)
				{
					InitAndLaunchSpecialAnimation(activeBattler, activeBattler, activeBattler, B_ANIM_MON_TO_SUBSTITUTE);
					gBattleSpritesDataPtr->bankData[activeBattler].substituteOffScreen = FALSE;
				}
				++gTasks[taskId].tState;
			}
			break;

		case 3:
			if (!gAnimScriptActive)
			{
				gBattleSpritesDataPtr->healthBoxesData[gTasks[taskId].tBattlerId].animFromTableActive = FALSE;
				DestroyTask(taskId);
			}
	}
}

static bool8 ShouldAnimBeDoneRegardlessOfSubsitute(u8 animId)
{
	switch (animId) {
		case B_ANIM_SUBSTITUTE_FADE:
		case B_ANIM_SNATCH_MOVE:
		case B_ANIM_LOAD_ABILITY_POP_UP:
		case B_ANIM_DESTROY_ABILITY_POP_UP:
		case B_ANIM_RAIN_CONTINUES:
		case B_ANIM_SUN_CONTINUES:
		case B_ANIM_SANDSTORM_CONTINUES:
		case B_ANIM_HAIL_CONTINUES:
		case B_ANIM_STRONG_WINDS_CONTINUE:
		case B_ANIM_FOG_CONTINUES:
		case B_ANIM_ELECTRIC_SURGE:
		case B_ANIM_GRASSY_SURGE:
		case B_ANIM_MISTY_SURGE:
		case B_ANIM_PSYCHIC_SURGE:
		case B_ELECTRIC_TERRAIN_ACTIVE_ANIM:
		case B_GRASSY_TERRAIN_ACTIVE_ANIM:
		case B_MISTY_TERRAIN_ACTIVE_ANIM:
		case B_PSYCHIC_TERRAIN_ACTIVE_ANIM:
		case B_ANIM_LOAD_DEFAULT_BG:
		case B_ANIM_TOTEM_BOOST:
		case B_ANIM_DYNAMAX_START:
		case B_ANIM_RAID_BATTLE_STORM:
		case B_ANIM_DYNAMAX_ENERGY_SWIRL:
			return TRUE;
		default:
			return FALSE;
	}
}

static bool8 ShouldSubstituteRecedeForSpecialBattleAnim(u8 animId)
{
	switch (animId) {
		case B_ANIM_TRANSFORM:
		case B_ANIM_WISHIWASHI_FISH:
		case B_ANIM_ZYGARDE_CELL_SWIRL:
		case B_ANIM_BLUE_PRIMAL_REVERSION:
		case B_ANIM_RED_PRIMAL_REVERSION:
		case B_ANIM_POWDER_EXPLOSION:
		case B_ANIM_BEAK_BLAST_WARM_UP:
		case B_ANIM_SHELL_TRAP_SET:
		case B_BATON_PASS_ANIM:
		case B_DRAGON_TAIL_BLOW_AWAY_ANIM:
		case B_ANIM_ZMOVE_ACTIVATE:
		case B_ANIM_MEGA_EVOLUTION:
		case B_ANIM_ULTRA_BURST:
		case B_ANIM_DYNAMAX_START:
		case B_ANIM_RAID_BATTLE_ENERGY_BURST:
			return TRUE;
		default:
			return FALSE;
	}
}

static void TrySwapBackupSpeciesWithSpecies(u8 activeBattler, u8 animId)
{
	struct Pokemon* animGuy = GetBankPartyData(activeBattler);

	switch (animId) {
		case B_ANIM_TRANSFORM:
		case B_ANIM_WISHIWASHI_FISH:
		case B_ANIM_ZYGARDE_CELL_SWIRL:
		case B_ANIM_BLUE_PRIMAL_REVERSION:
		case B_ANIM_RED_PRIMAL_REVERSION:
		case B_ANIM_MEGA_EVOLUTION:
		case B_ANIM_ULTRA_BURST:
			if (animGuy->backupSpecies != SPECIES_NONE)
			{
				u16 backup = animGuy->species;
				animGuy->species = animGuy->backupSpecies;
				animGuy->backupSpecies = backup;
			}
	}
}

//Critical Capture Animations
void SpriteCB_CriticalCaptureThrownBallMovement(struct Sprite* sprite)
{
	bool8 lastBounce = FALSE;
	u8 maxBounces = 6;
	int bounceCount;

	bounceCount = sprite->data[3] >> 8;

	if (bounceCount == 0)
		PlaySE(SE_POKE_BALL_SHAKE);

	switch (sprite->data[3] & 0xFF)
	{
	case 0:
		if (bounceCount < 3)
			sprite->pos2.x++;

		sprite->data[5]++;
		if (sprite->data[5] >= 3)
			sprite->data[3] += 257;

		break;
	case 1:
		if (bounceCount < 3 || sprite->pos2.x != 0)
			sprite->pos2.x--;

		sprite->data[5]--;

		if (sprite->data[5] <= 0)
		{
			sprite->data[5] = 0;
			sprite->data[3] &= -0x100;
		}

		if (bounceCount >= maxBounces)
			lastBounce = TRUE;

		break;
	}

	if (lastBounce)
	{

		sprite->data[3] = 0;
		sprite->data[4] = 40;   //Starting max height
		sprite->data[5] = 0;
		sprite->callback = SpriteCB_ThrowBallMovement;
	}
}

void SpriteCB_InitThrownBallBouncing(struct Sprite *sprite)
{
	int angle;

	if (sprite->animEnded)
	{
		sprite->data[3] = 0;
		sprite->data[4] = 40;
		sprite->data[5] = 0;
		angle = 0;
		sprite->pos1.y += Cosine(angle, 40);
		sprite->pos2.y = -Cosine(angle, sprite->data[4]);
		if (gNewBS->criticalCapture)
			sprite->callback = SpriteCB_CriticalCaptureThrownBallMovement;
		else
			sprite->callback = SpriteCB_ThrowBallMovement;
	}
}
