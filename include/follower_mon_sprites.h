#ifndef FOLLOWER_MON_SPRITES_H
#define FOLLOWER_MON_SPRITES_H

#include "global.h"
#include "constants/event_objects.h"

#define EVENT_OBJ_PAL_TAG_NONE 0x1102
#define gEventObjectBaseOam_16x16 ((const struct OamData*) 0x83A36F8)
#define gEventObjectBaseOam_16x32 ((const struct OamData*) 0x83A3710)
#define gEventObjectBaseOam_32x32 ((const struct OamData*) 0x83A3718)
#define gEventObjectBaseOam_64x64 ((const struct OamData*) 0x83A3720)
#define gEventObjectSpriteOamTables_16x16 ((const struct SubspriteTable*) 0x83A3748)
#define gEventObjectSpriteOamTables_16x32 ((const struct SubspriteTable*) 0x83A379C)
#define gEventObjectSpriteOamTables_32x32 ((const struct SubspriteTable*) 0x83A37F0)
#define gEventObjectSpriteOamTables_64x64 ((const struct SubspriteTable*) 0x83A38D0)
#define gEventObjectImageAnimTable_PlayerNormal ((const union AnimCmd* const*) 0x83A3470)
#define gEventObjectImageAnimTable_Standard ((const union AnimCmd* const*) 0x83A3368)
#define gEventObjectImageAnimTable_Surfing ((const union AnimCmd* const*) 0x83A3584)
#define gEventObjectImageAnimTable_FieldMove ((const union AnimCmd* const*) 0x83A3638)
#define gEventObjectImageAnimTable_Fishing ((const union AnimCmd* const*) 0x83A3668)
#define gEventObjectImageAnimTable_VsSeekerBike ((const union AnimCmd* const*) 0x83A3640)

#define ANIM_STD_FACE_SOUTH       0
#define ANIM_STD_FACE_NORTH       1
#define ANIM_STD_FACE_WEST        2
#define ANIM_STD_FACE_EAST        3
#define ANIM_STD_GO_SOUTH         4
#define ANIM_STD_GO_NORTH         5
#define ANIM_STD_GO_WEST          6
#define ANIM_STD_GO_EAST          7
#define ANIM_STD_GO_FAST_SOUTH    8
#define ANIM_STD_GO_FAST_NORTH    9
#define ANIM_STD_GO_FAST_WEST     10
#define ANIM_STD_GO_FAST_EAST     11
#define ANIM_STD_GO_FASTER_SOUTH  12
#define ANIM_STD_GO_FASTER_NORTH  13
#define ANIM_STD_GO_FASTER_WEST   14
#define ANIM_STD_GO_FASTER_EAST   15
#define ANIM_STD_GO_FASTEST_SOUTH 16
#define ANIM_STD_GO_FASTEST_NORTH 17
#define ANIM_STD_GO_FASTEST_WEST  18
#define ANIM_STD_GO_FASTEST_EAST  19
#define ANIM_RAISE_HAND           20

#define MON_OW_TEMPLATE_32x32_FRAMES(Id, name)                        \
static const struct SpriteFrameImage sMonPicTable_##name[] =          \
{                                                                     \
    overworld_frame(gFollowingMonPic_##Id##_##name##Tiles, 4, 4, 0),    \
    overworld_frame(gFollowingMonPic_##Id##_##name##Tiles, 4, 4, 1),    \
    overworld_frame(gFollowingMonPic_##Id##_##name##Tiles, 4, 4, 2),    \
    overworld_frame(gFollowingMonPic_##Id##_##name##Tiles, 4, 4, 3),    \
    overworld_frame(gFollowingMonPic_##Id##_##name##Tiles, 4, 4, 4),    \
    overworld_frame(gFollowingMonPic_##Id##_##name##Tiles, 4, 4, 5),    \
    overworld_frame(gFollowingMonPic_##Id##_##name##Tiles, 4, 4, 6),    \
    overworld_frame(gFollowingMonPic_##Id##_##name##Tiles, 4, 4, 7),    \
    overworld_frame(gFollowingMonPic_##Id##_##name##Tiles, 4, 4, 8),    \
};

#define MON_OW_OBJECT_GRAPHICS(PalId, name)                 \
{                                                           \
    .tileTag = 0xFFFF,                                      \
    .paletteTag1 = PalId,                               \
    .paletteTag2 = EVENT_OBJ_PAL_TAG_NONE,                  \
    .size = (32 * 32) / 2,                                  \
    .width = 32,                                            \
    .height = 32,                                           \
    .shadowSize = SHADOW_SIZE_M,                            \
    .inanimate = FALSE,                                     \
    .disableReflectionPaletteLoad = FALSE,                  \
    .tracks = TRACKS_FOOT,                                  \
    .gender = MALE,                                         \
    .oam = gEventObjectBaseOam_32x32,                       \
    .subspriteTables = gEventObjectSpriteOamTables_32x32,   \
    .anims = gFollowerMonAnimTable,                         \
    .images = sMonPicTable_##name,                          \
    .affineAnims = gDummySpriteAffineAnimTable,             \
},

#define MON_OW_TEMPLATE_64x64_FRAMES(id, name)                          \
static const struct SpriteFrameImage sMonPicTable_##name[] = {             \
    overworld_frame(gFollowingMonPic_##id##_##name##Tiles, 8, 8, 0),    \
    overworld_frame(gFollowingMonPic_##id##_##name##Tiles, 8, 8, 1),    \
    overworld_frame(gFollowingMonPic_##id##_##name##Tiles, 8, 8, 2),    \
    overworld_frame(gFollowingMonPic_##id##_##name##Tiles, 8, 8, 3),    \
    overworld_frame(gFollowingMonPic_##id##_##name##Tiles, 8, 8, 4),    \
    overworld_frame(gFollowingMonPic_##id##_##name##Tiles, 8, 8, 5),    \
};

#define MON_OW_OBJECT_GRAPHICS_64(PalId, name)                          \
{                                                                       \
        .tileTag = 0xFFFF,                                              \
        .paletteTag1 = PalId,                                           \
        .paletteTag2 = EVENT_OBJ_PAL_TAG_NONE,                          \
        .size = (64 * 64) / 2,                                          \
        .width = 64,                                                    \
        .height = 64,                                                   \
        .shadowSize = SHADOW_SIZE_M,                                    \
        .inanimate = FALSE,                                             \
        .disableReflectionPaletteLoad = FALSE,                          \
        .tracks = TRACKS_FOOT,                                          \
        .gender = MALE,                                                 \
        .oam = gEventObjectBaseOam_64x64,                               \
        .subspriteTables = gEventObjectSpriteOamTables_64x64,           \
        .anims = gFollowerMonAnimTable,                                \
        .images = sMonPicTable_##name,                                     \
        .affineAnims = gDummySpriteAffineAnimTable,                     \
},

extern const u16 gFollowerMonSpriteIdTable[];
extern const union AnimCmd *const gFollowerMonAnimTable[];

// Overworld Tables
extern const struct EventObjectGraphicsInfo gFollowerMonGfxTable0[];
extern const struct EventObjectGraphicsInfo gFollowerMonGfxTable1[];
extern const struct EventObjectGraphicsInfo gFollowerMonGfxTable2[];
extern const struct EventObjectGraphicsInfo gFollowerMonGfxTable3[];
extern const struct EventObjectGraphicsInfo gFollowerMonGfxTable4[];

//Tiles
#define gFollowingMonPic_0001_BulbasaurTiles ((u8*)0x08900000)
#define gFollowingMonPic_0002_IvysaurTiles ((u8*)0x08900C00)
#define gFollowingMonPic_0003_VenusaurTiles ((u8*)0x08901800)
#define gFollowingMonPic_0004_CharmanderTiles ((u8*)0x08902400)
#define gFollowingMonPic_0005_CharmeleonTiles ((u8*)0x08903000)
#define gFollowingMonPic_0006_CharizardTiles ((u8*)0x08903C00)
#define gFollowingMonPic_0007_SquirtleTiles ((u8*)0x08904800)
#define gFollowingMonPic_0008_WartortleTiles ((u8*)0x08905400)
#define gFollowingMonPic_0009_BlastoiseTiles ((u8*)0x08906000)
#define gFollowingMonPic_0010_CaterpieTiles ((u8*)0x08906C00)
#define gFollowingMonPic_0011_MetapodTiles ((u8*)0x08907800)
#define gFollowingMonPic_0012_ButterfreeTiles ((u8*)0x08908400)
#define gFollowingMonPic_0013_WeedleTiles ((u8*)0x08909000)
#define gFollowingMonPic_0014_KakunaTiles ((u8*)0x08909C00)
#define gFollowingMonPic_0015_BeedrillTiles ((u8*)0x0890A800)
#define gFollowingMonPic_0016_PidgeyTiles ((u8*)0x0890B400)
#define gFollowingMonPic_0017_PidgeottoTiles ((u8*)0x0890C000)
#define gFollowingMonPic_0018_PidgeotTiles ((u8*)0x0890CC00)
#define gFollowingMonPic_0019_RattataTiles ((u8*)0x0890D800)
#define gFollowingMonPic_0020_RaticateTiles ((u8*)0x0890E400)
#define gFollowingMonPic_0021_SpearowTiles ((u8*)0x0890F000)
#define gFollowingMonPic_0022_FearowTiles ((u8*)0x0890FC00)
#define gFollowingMonPic_0023_EkansTiles ((u8*)0x08910800)
#define gFollowingMonPic_0024_ArbokTiles ((u8*)0x08911400)
#define gFollowingMonPic_0025_PikachuTiles ((u8*)0x08912000)
#define gFollowingMonPic_0026_RaichuTiles ((u8*)0x08912C00)
#define gFollowingMonPic_0027_SandshrewTiles ((u8*)0x08913800)
#define gFollowingMonPic_0028_SandslashTiles ((u8*)0x08914400)
#define gFollowingMonPic_0029_Nidoran_FTiles ((u8*)0x08915000)
#define gFollowingMonPic_0030_NidorinaTiles ((u8*)0x08915C00)
#define gFollowingMonPic_0031_NidoqueenTiles ((u8*)0x08916800)
#define gFollowingMonPic_0032_Nidoran_MTiles ((u8*)0x08917400)
#define gFollowingMonPic_0033_NidorinoTiles ((u8*)0x08918000)
#define gFollowingMonPic_0034_NidokingTiles ((u8*)0x08918C00)
#define gFollowingMonPic_0035_ClefairyTiles ((u8*)0x08919800)
#define gFollowingMonPic_0036_ClefableTiles ((u8*)0x0891A400)
#define gFollowingMonPic_0037_VulpixTiles ((u8*)0x0891B000)
#define gFollowingMonPic_0038_NinetalesTiles ((u8*)0x0891BC00)
#define gFollowingMonPic_0039_JigglypuffTiles ((u8*)0x0891C800)
#define gFollowingMonPic_0040_WigglytuffTiles ((u8*)0x0891D400)
#define gFollowingMonPic_0041_ZubatTiles ((u8*)0x0891E000)
#define gFollowingMonPic_0042_GolbatTiles ((u8*)0x0891EC00)
#define gFollowingMonPic_0043_OddishTiles ((u8*)0x0891F800)
#define gFollowingMonPic_0044_GloomTiles ((u8*)0x08920400)
#define gFollowingMonPic_0045_VileplumeTiles ((u8*)0x08921000)
#define gFollowingMonPic_0046_ParasTiles ((u8*)0x08921C00)
#define gFollowingMonPic_0047_ParasectTiles ((u8*)0x08922800)
#define gFollowingMonPic_0048_VenonatTiles ((u8*)0x08923400)
#define gFollowingMonPic_0049_VenomothTiles ((u8*)0x08924000)
#define gFollowingMonPic_0050_DiglettTiles ((u8*)0x08924C00)
#define gFollowingMonPic_0051_DugtrioTiles ((u8*)0x08925800)
#define gFollowingMonPic_0052_MeowthTiles ((u8*)0x08926400)
#define gFollowingMonPic_0053_PersianTiles ((u8*)0x08927000)
#define gFollowingMonPic_0054_PsyduckTiles ((u8*)0x08927C00)
#define gFollowingMonPic_0055_GolduckTiles ((u8*)0x08928800)
#define gFollowingMonPic_0056_MankeyTiles ((u8*)0x08929400)
#define gFollowingMonPic_0057_PrimeapeTiles ((u8*)0x0892A000)
#define gFollowingMonPic_0058_GrowlitheTiles ((u8*)0x0892AC00)
#define gFollowingMonPic_0059_ArcanineTiles ((u8*)0x0892B800)
#define gFollowingMonPic_0060_PoliwagTiles ((u8*)0x0892C400)
#define gFollowingMonPic_0061_PoliwhirlTiles ((u8*)0x0892D000)
#define gFollowingMonPic_0062_PoliwrathTiles ((u8*)0x0892DC00)
#define gFollowingMonPic_0063_AbraTiles ((u8*)0x0892E800)
#define gFollowingMonPic_0064_KadabraTiles ((u8*)0x0892F400)
#define gFollowingMonPic_0065_AlakazamTiles ((u8*)0x08930000)
#define gFollowingMonPic_0066_MachopTiles ((u8*)0x08930C00)
#define gFollowingMonPic_0067_MachokeTiles ((u8*)0x08931800)
#define gFollowingMonPic_0068_MachampTiles ((u8*)0x08932400)
#define gFollowingMonPic_0069_BellsproutTiles ((u8*)0x08933000)
#define gFollowingMonPic_0070_WeepinbellTiles ((u8*)0x08933C00)
#define gFollowingMonPic_0071_VictreebelTiles ((u8*)0x08934800)
#define gFollowingMonPic_0072_TentacoolTiles ((u8*)0x08935400)
#define gFollowingMonPic_0073_TentacruelTiles ((u8*)0x08936000)
#define gFollowingMonPic_0074_GeodudeTiles ((u8*)0x08936C00)
#define gFollowingMonPic_0075_GravelerTiles ((u8*)0x08937800)
#define gFollowingMonPic_0076_GolemTiles ((u8*)0x08938400)
#define gFollowingMonPic_0077_PonytaTiles ((u8*)0x08939000)
#define gFollowingMonPic_0078_RapidashTiles ((u8*)0x08939C00)
#define gFollowingMonPic_0079_SlowpokeTiles ((u8*)0x0893A800)
#define gFollowingMonPic_0080_SlowbroTiles ((u8*)0x0893B400)
#define gFollowingMonPic_0081_MagnemiteTiles ((u8*)0x0893C000)
#define gFollowingMonPic_0082_MagnetonTiles ((u8*)0x0893CC00)
#define gFollowingMonPic_0083_FarfetchdTiles ((u8*)0x0893D800)
#define gFollowingMonPic_0084_DoduoTiles ((u8*)0x0893E800)
#define gFollowingMonPic_0085_DodrioTiles ((u8*)0x0893F400)
#define gFollowingMonPic_0086_SeelTiles ((u8*)0x08940000)
#define gFollowingMonPic_0087_DewgongTiles ((u8*)0x08940C00)
#define gFollowingMonPic_0088_GrimerTiles ((u8*)0x08941800)
#define gFollowingMonPic_0089_MukTiles ((u8*)0x08942400)
#define gFollowingMonPic_0090_ShellderTiles ((u8*)0x08943000)
#define gFollowingMonPic_0091_CloysterTiles ((u8*)0x08943C00)
#define gFollowingMonPic_0092_GastlyTiles ((u8*)0x08944800)
#define gFollowingMonPic_0093_HaunterTiles ((u8*)0x08945400)
#define gFollowingMonPic_0094_GengarTiles ((u8*)0x08946000)
#define gFollowingMonPic_0095_OnixTiles ((u8*)0x08946C00)
#define gFollowingMonPic_0096_DrowzeeTiles ((u8*)0x08947800)
#define gFollowingMonPic_0097_HypnoTiles ((u8*)0x08948400)
#define gFollowingMonPic_0098_KrabbyTiles ((u8*)0x08949000)
#define gFollowingMonPic_0099_KinglerTiles ((u8*)0x08949C00)
#define gFollowingMonPic_0100_VoltorbTiles ((u8*)0x0894A800)
#define gFollowingMonPic_0101_ElectrodeTiles ((u8*)0x0894B400)
#define gFollowingMonPic_0102_ExeggcuteTiles ((u8*)0x0894C000)
#define gFollowingMonPic_0103_ExeggutorTiles ((u8*)0x0894CC00)
#define gFollowingMonPic_0104_CuboneTiles ((u8*)0x0894D800)
#define gFollowingMonPic_0105_MarowakTiles ((u8*)0x0894E400)
#define gFollowingMonPic_0106_HitmonleeTiles ((u8*)0x0894F000)
#define gFollowingMonPic_0107_HitmonchanTiles ((u8*)0x0894FC00)
#define gFollowingMonPic_0108_LickitungTiles ((u8*)0x08950800)
#define gFollowingMonPic_0109_KoffingTiles ((u8*)0x08951400)
#define gFollowingMonPic_0110_WeezingTiles ((u8*)0x08952000)
#define gFollowingMonPic_0111_RhyhornTiles ((u8*)0x08952C00)
#define gFollowingMonPic_0112_RhydonTiles ((u8*)0x08953800)
#define gFollowingMonPic_0113_ChanseyTiles ((u8*)0x08954400)
#define gFollowingMonPic_0114_TangelaTiles ((u8*)0x08955000)
#define gFollowingMonPic_0115_KangaskhanTiles ((u8*)0x08955C00)
#define gFollowingMonPic_0116_HorseaTiles ((u8*)0x08956800)
#define gFollowingMonPic_0117_SeadraTiles ((u8*)0x08957400)
#define gFollowingMonPic_0118_GoldeenTiles ((u8*)0x08958000)
#define gFollowingMonPic_0119_SeakingTiles ((u8*)0x08958C00)
#define gFollowingMonPic_0120_StaryuTiles ((u8*)0x08959800)
#define gFollowingMonPic_0121_StarmieTiles ((u8*)0x0895A400)
#define gFollowingMonPic_0122_Mr_MimeTiles ((u8*)0x0895B000)
#define gFollowingMonPic_0123_ScytherTiles ((u8*)0x0895BC00)
#define gFollowingMonPic_0124_JynxTiles ((u8*)0x0895C800)
#define gFollowingMonPic_0125_ElectabuzzTiles ((u8*)0x0895D400)
#define gFollowingMonPic_0126_MagmarTiles ((u8*)0x0895E000)
#define gFollowingMonPic_0127_PinsirTiles ((u8*)0x0895EC00)
#define gFollowingMonPic_0128_TaurosTiles ((u8*)0x0895F800)
#define gFollowingMonPic_0129_MagikarpTiles ((u8*)0x08960400)
#define gFollowingMonPic_0130_GyaradosTiles ((u8*)0x08961000)
#define gFollowingMonPic_0131_LaprasTiles ((u8*)0x08961C00)
#define gFollowingMonPic_0132_DittoTiles ((u8*)0x08962800)
#define gFollowingMonPic_0133_EeveeTiles ((u8*)0x08963400)
#define gFollowingMonPic_0134_VaporeonTiles ((u8*)0x08964000)
#define gFollowingMonPic_0135_JolteonTiles ((u8*)0x08964C00)
#define gFollowingMonPic_0136_FlareonTiles ((u8*)0x08965800)
#define gFollowingMonPic_0137_PorygonTiles ((u8*)0x08966400)
#define gFollowingMonPic_0138_OmanyteTiles ((u8*)0x08967000)
#define gFollowingMonPic_0139_OmastarTiles ((u8*)0x08967C00)
#define gFollowingMonPic_0140_KabutoTiles ((u8*)0x08968800)
#define gFollowingMonPic_0141_KabutopsTiles ((u8*)0x08969400)
#define gFollowingMonPic_0142_AerodactylTiles ((u8*)0x0896A000)
#define gFollowingMonPic_0143_SnorlaxTiles ((u8*)0x0896AC00)
#define gFollowingMonPic_0144_ArticunoTiles ((u8*)0x0896B800)
#define gFollowingMonPic_0145_ZapdosTiles ((u8*)0x0896C400)
#define gFollowingMonPic_0146_MoltresTiles ((u8*)0x0896D000)
#define gFollowingMonPic_0147_DratiniTiles ((u8*)0x0896DC00)
#define gFollowingMonPic_0148_DragonairTiles ((u8*)0x0896E800)
#define gFollowingMonPic_0149_DragoniteTiles ((u8*)0x0896F400)
#define gFollowingMonPic_0150_MewtwoTiles ((u8*)0x08970000)
#define gFollowingMonPic_0151_MewTiles ((u8*)0x08970C00)
#define gFollowingMonPic_0152_ChikoritaTiles ((u8*)0x08971800)
#define gFollowingMonPic_0153_BayleefTiles ((u8*)0x08972400)
#define gFollowingMonPic_0154_MeganiumTiles ((u8*)0x08973000)
#define gFollowingMonPic_0155_CyndaquilTiles ((u8*)0x08973C00)
#define gFollowingMonPic_0156_QuilavaTiles ((u8*)0x08974800)
#define gFollowingMonPic_0157_TyphlosionTiles ((u8*)0x08975400)
#define gFollowingMonPic_0158_TotodileTiles ((u8*)0x08976000)
#define gFollowingMonPic_0159_CroconawTiles ((u8*)0x08976C00)
#define gFollowingMonPic_0160_FeraligatrTiles ((u8*)0x08977800)
#define gFollowingMonPic_0161_SentretTiles ((u8*)0x08978400)
#define gFollowingMonPic_0162_FurretTiles ((u8*)0x08979000)
#define gFollowingMonPic_0163_HoothootTiles ((u8*)0x08979C00)
#define gFollowingMonPic_0164_NoctowlTiles ((u8*)0x0897A800)
#define gFollowingMonPic_0165_LedybaTiles ((u8*)0x0897B400)
#define gFollowingMonPic_0166_LedianTiles ((u8*)0x0897C000)
#define gFollowingMonPic_0167_SpinarakTiles ((u8*)0x0897CC00)
#define gFollowingMonPic_0168_AriadosTiles ((u8*)0x0897D800)
#define gFollowingMonPic_0169_CrobatTiles ((u8*)0x0897E400)
#define gFollowingMonPic_0170_ChinchouTiles ((u8*)0x0897F000)
#define gFollowingMonPic_0171_LanturnTiles ((u8*)0x0897FC00)
#define gFollowingMonPic_0172_PichuTiles ((u8*)0x08980800)
#define gFollowingMonPic_0173_CleffaTiles ((u8*)0x08981400)
#define gFollowingMonPic_0174_IgglybuffTiles ((u8*)0x08982000)
#define gFollowingMonPic_0175_TogepiTiles ((u8*)0x08982C00)
#define gFollowingMonPic_0176_TogeticTiles ((u8*)0x08983800)
#define gFollowingMonPic_0177_NatuTiles ((u8*)0x08984400)
#define gFollowingMonPic_0178_XatuTiles ((u8*)0x08985000)
#define gFollowingMonPic_0179_MareepTiles ((u8*)0x08985C00)
#define gFollowingMonPic_0180_FlaaffyTiles ((u8*)0x08986800)
#define gFollowingMonPic_0181_AmpharosTiles ((u8*)0x08987400)
#define gFollowingMonPic_0182_BellossomTiles ((u8*)0x08988000)
#define gFollowingMonPic_0183_MarillTiles ((u8*)0x08988C00)
#define gFollowingMonPic_0184_AzumarillTiles ((u8*)0x08989800)
#define gFollowingMonPic_0185_SudowoodoTiles ((u8*)0x0898A400)
#define gFollowingMonPic_0186_PolitoedTiles ((u8*)0x0898B000)
#define gFollowingMonPic_0187_HoppipTiles ((u8*)0x0898BC00)
#define gFollowingMonPic_0188_SkiploomTiles ((u8*)0x0898C800)
#define gFollowingMonPic_0189_JumpluffTiles ((u8*)0x0898D400)
#define gFollowingMonPic_0190_AipomTiles ((u8*)0x0898E000)
#define gFollowingMonPic_0191_SunkernTiles ((u8*)0x0898EC00)
#define gFollowingMonPic_0192_SunfloraTiles ((u8*)0x0898F800)
#define gFollowingMonPic_0193_YanmaTiles ((u8*)0x08990400)
#define gFollowingMonPic_0194_WooperTiles ((u8*)0x08991000)
#define gFollowingMonPic_0195_QuagsireTiles ((u8*)0x08991C00)
#define gFollowingMonPic_0196_EspeonTiles ((u8*)0x08992800)
#define gFollowingMonPic_0197_UmbreonTiles ((u8*)0x08993400)
#define gFollowingMonPic_0198_MurkrowTiles ((u8*)0x08994000)
#define gFollowingMonPic_0199_SlowkingTiles ((u8*)0x08994C00)
#define gFollowingMonPic_0200_MisdreavusTiles ((u8*)0x08995800)
#define gFollowingMonPic_0201_UnownTiles ((u8*)0x08996400)
#define gFollowingMonPic_0202_WobbuffetTiles ((u8*)0x08997000)
#define gFollowingMonPic_0203_GirafarigTiles ((u8*)0x08997C00)
#define gFollowingMonPic_0204_PinecoTiles ((u8*)0x08998800)
#define gFollowingMonPic_0205_ForretressTiles ((u8*)0x08999400)
#define gFollowingMonPic_0206_DunsparceTiles ((u8*)0x0899A000)
#define gFollowingMonPic_0207_GligarTiles ((u8*)0x0899AC00)
#define gFollowingMonPic_0208_SteelixTiles ((u8*)0x0899B800)
#define gFollowingMonPic_0209_SnubbullTiles ((u8*)0x0899E800)
#define gFollowingMonPic_0210_GranbullTiles ((u8*)0x0899F400)
#define gFollowingMonPic_0211_QwilfishTiles ((u8*)0x089A0000)
#define gFollowingMonPic_0212_ScizorTiles ((u8*)0x089A0C00)
#define gFollowingMonPic_0213_ShuckleTiles ((u8*)0x089A1800)
#define gFollowingMonPic_0214_HeracrossTiles ((u8*)0x089A2400)
#define gFollowingMonPic_0215_SneaselTiles ((u8*)0x089A3000)
#define gFollowingMonPic_0216_TeddiursaTiles ((u8*)0x089A3C00)
#define gFollowingMonPic_0217_UrsaringTiles ((u8*)0x089A4800)
#define gFollowingMonPic_0218_SlugmaTiles ((u8*)0x089A5400)
#define gFollowingMonPic_0219_MagcargoTiles ((u8*)0x089A6000)
#define gFollowingMonPic_0220_SwinubTiles ((u8*)0x089A6C00)
#define gFollowingMonPic_0221_PiloswineTiles ((u8*)0x089A7800)
#define gFollowingMonPic_0222_CorsolaTiles ((u8*)0x089A8400)
#define gFollowingMonPic_0223_RemoraidTiles ((u8*)0x089A9000)
#define gFollowingMonPic_0224_OctilleryTiles ((u8*)0x089A9C00)
#define gFollowingMonPic_0225_DelibirdTiles ((u8*)0x089AA800)
#define gFollowingMonPic_0226_MantineTiles ((u8*)0x089AB400)
#define gFollowingMonPic_0227_SkarmoryTiles ((u8*)0x089AC000)
#define gFollowingMonPic_0228_HoundourTiles ((u8*)0x089ACC00)
#define gFollowingMonPic_0229_HoundoomTiles ((u8*)0x089AD800)
#define gFollowingMonPic_0230_KingdraTiles ((u8*)0x089AE400)
#define gFollowingMonPic_0231_PhanpyTiles ((u8*)0x089AF000)
#define gFollowingMonPic_0232_DonphanTiles ((u8*)0x089AFC00)
#define gFollowingMonPic_0233_Porygon2Tiles ((u8*)0x089B0800)
#define gFollowingMonPic_0234_StantlerTiles ((u8*)0x089B1400)
#define gFollowingMonPic_0235_SmeargleTiles ((u8*)0x089B2000)
#define gFollowingMonPic_0236_TyrogueTiles ((u8*)0x089B2C00)
#define gFollowingMonPic_0237_HitmontopTiles ((u8*)0x089B3800)
#define gFollowingMonPic_0238_SmoochumTiles ((u8*)0x089B4400)
#define gFollowingMonPic_0239_ElekidTiles ((u8*)0x089B5000)
#define gFollowingMonPic_0240_MagbyTiles ((u8*)0x089B5C00)
#define gFollowingMonPic_0241_MiltankTiles ((u8*)0x089B6800)
#define gFollowingMonPic_0242_BlisseyTiles ((u8*)0x089B7400)
#define gFollowingMonPic_0243_RaikouTiles ((u8*)0x089B8000)
#define gFollowingMonPic_0244_EnteiTiles ((u8*)0x089B8C00)
#define gFollowingMonPic_0245_SuicuneTiles ((u8*)0x089B9800)
#define gFollowingMonPic_0246_LarvitarTiles ((u8*)0x089BA400)
#define gFollowingMonPic_0247_PupitarTiles ((u8*)0x089BB000)
#define gFollowingMonPic_0248_TyranitarTiles ((u8*)0x089BBC00)
#define gFollowingMonPic_0249_DLugiaTiles ((u8*)0x089BC800)
#define gFollowingMonPic_0249_LugiaTiles ((u8*)0x089BF800)
#define gFollowingMonPic_0250_Ho_OhTiles ((u8*)0x089C2800)
#define gFollowingMonPic_0251_CelebiTiles ((u8*)0x089C5800)
#define gFollowingMonPic_0277_TreeckoTiles ((u8*)0x089C6400)
#define gFollowingMonPic_0278_GrovyleTiles ((u8*)0x089C7000)
#define gFollowingMonPic_0279_SceptileTiles ((u8*)0x089C7C00)
#define gFollowingMonPic_0280_TorchicTiles ((u8*)0x089C8800)
#define gFollowingMonPic_0281_CombuskenTiles ((u8*)0x089C9400)
#define gFollowingMonPic_0282_BlazikenTiles ((u8*)0x089CA000)
#define gFollowingMonPic_0283_MudkipTiles ((u8*)0x089CAC00)
#define gFollowingMonPic_0284_MarshtompTiles ((u8*)0x089CB800)
#define gFollowingMonPic_0285_SwampertTiles ((u8*)0x089CC400)
#define gFollowingMonPic_0286_PoochyenaTiles ((u8*)0x089CD000)
#define gFollowingMonPic_0287_MightyenaTiles ((u8*)0x089CDC00)
#define gFollowingMonPic_0288_ZigzagoonTiles ((u8*)0x089CE800)
#define gFollowingMonPic_0289_LinooneTiles ((u8*)0x089CF400)
#define gFollowingMonPic_0290_WurmpleTiles ((u8*)0x089D0000)
#define gFollowingMonPic_0291_SilcoonTiles ((u8*)0x089D0C00)
#define gFollowingMonPic_0292_BeautiflyTiles ((u8*)0x089D1800)
#define gFollowingMonPic_0293_CascoonTiles ((u8*)0x089D2400)
#define gFollowingMonPic_0294_DustoxTiles ((u8*)0x089D3000)
#define gFollowingMonPic_0295_LotadTiles ((u8*)0x089D3C00)
#define gFollowingMonPic_0296_LombreTiles ((u8*)0x089D4800)
#define gFollowingMonPic_0297_LudicoloTiles ((u8*)0x089D5400)
#define gFollowingMonPic_0298_SeedotTiles ((u8*)0x089D6000)
#define gFollowingMonPic_0299_NuzleafTiles ((u8*)0x089D6C00)
#define gFollowingMonPic_0300_ShiftryTiles ((u8*)0x089D7800)
#define gFollowingMonPic_0301_NincadaTiles ((u8*)0x089D8400)
#define gFollowingMonPic_0302_NinjaskTiles ((u8*)0x089D9000)
#define gFollowingMonPic_0303_ShedinjaTiles ((u8*)0x089D9C00)
#define gFollowingMonPic_0304_TaillowTiles ((u8*)0x089DA800)
#define gFollowingMonPic_0305_SwellowTiles ((u8*)0x089DB400)
#define gFollowingMonPic_0306_ShroomishTiles ((u8*)0x089DC000)
#define gFollowingMonPic_0307_BreloomTiles ((u8*)0x089DCC00)
#define gFollowingMonPic_0308_SpindaTiles ((u8*)0x089DD800)
#define gFollowingMonPic_0309_WingullTiles ((u8*)0x089DE400)
#define gFollowingMonPic_0310_PelipperTiles ((u8*)0x089DF000)
#define gFollowingMonPic_0311_SurskitTiles ((u8*)0x089DFC00)
#define gFollowingMonPic_0312_MasquerainTiles ((u8*)0x089E0800)
#define gFollowingMonPic_0313_WailmerTiles ((u8*)0x089E1400)
#define gFollowingMonPic_0314_WailordTiles ((u8*)0x089E2000)
#define gFollowingMonPic_0315_SkittyTiles ((u8*)0x089E5000)
#define gFollowingMonPic_0316_DelcattyTiles ((u8*)0x089E5C00)
#define gFollowingMonPic_0317_KecleonTiles ((u8*)0x089E6800)
#define gFollowingMonPic_0318_BaltoyTiles ((u8*)0x089E7400)
#define gFollowingMonPic_0319_ClaydolTiles ((u8*)0x089E8000)
#define gFollowingMonPic_0320_NosepassTiles ((u8*)0x089E8C00)
#define gFollowingMonPic_0321_TorkoalTiles ((u8*)0x089E9800)
#define gFollowingMonPic_0322_SableyeTiles ((u8*)0x089EA400)
#define gFollowingMonPic_0323_BarboachTiles ((u8*)0x089EB000)
#define gFollowingMonPic_0324_WhiscashTiles ((u8*)0x089EBC00)
#define gFollowingMonPic_0325_LuvdiscTiles ((u8*)0x089EC800)
#define gFollowingMonPic_0326_CorphishTiles ((u8*)0x089ED400)
#define gFollowingMonPic_0327_CrawdauntTiles ((u8*)0x089EE000)
#define gFollowingMonPic_0328_FeebasTiles ((u8*)0x089EEC00)
#define gFollowingMonPic_0329_MiloticTiles ((u8*)0x089EF800)
#define gFollowingMonPic_0330_CarvanhaTiles ((u8*)0x089F0400)
#define gFollowingMonPic_0331_SharpedoTiles ((u8*)0x089F1000)
#define gFollowingMonPic_0332_TrapinchTiles ((u8*)0x089F1C00)
#define gFollowingMonPic_0333_VibravaTiles ((u8*)0x089F2800)
#define gFollowingMonPic_0334_FlygonTiles ((u8*)0x089F3400)
#define gFollowingMonPic_0335_MakuhitaTiles ((u8*)0x089F4000)
#define gFollowingMonPic_0336_HariyamaTiles ((u8*)0x089F4C00)
#define gFollowingMonPic_0337_ElectrikeTiles ((u8*)0x089F5800)
#define gFollowingMonPic_0338_ManectricTiles ((u8*)0x089F6400)
#define gFollowingMonPic_0339_NumelTiles ((u8*)0x089F7000)
#define gFollowingMonPic_0340_CameruptTiles ((u8*)0x089F7C00)
#define gFollowingMonPic_0341_SphealTiles ((u8*)0x089F8800)
#define gFollowingMonPic_0342_SealeoTiles ((u8*)0x089F9400)
#define gFollowingMonPic_0343_WalreinTiles ((u8*)0x089FA000)
#define gFollowingMonPic_0344_CacneaTiles ((u8*)0x089FAC00)
#define gFollowingMonPic_0345_CacturneTiles ((u8*)0x089FB800)
#define gFollowingMonPic_0346_SnoruntTiles ((u8*)0x089FC400)
#define gFollowingMonPic_0347_GlalieTiles ((u8*)0x089FD000)
#define gFollowingMonPic_0348_LunatoneTiles ((u8*)0x089FDC00)
#define gFollowingMonPic_0349_SolrockTiles ((u8*)0x089FE800)
#define gFollowingMonPic_0350_AzurillTiles ((u8*)0x089FF400)
#define gFollowingMonPic_0351_SpoinkTiles ((u8*)0x08A00000)
#define gFollowingMonPic_0352_GrumpigTiles ((u8*)0x08A00C00)
#define gFollowingMonPic_0353_PlusleTiles ((u8*)0x08A01800)
#define gFollowingMonPic_0354_MinunTiles ((u8*)0x08A02400)
#define gFollowingMonPic_0355_MawileTiles ((u8*)0x08A03000)
#define gFollowingMonPic_0356_MedititeTiles ((u8*)0x08A03C00)
#define gFollowingMonPic_0357_MedichamTiles ((u8*)0x08A04800)
#define gFollowingMonPic_0358_SwabluTiles ((u8*)0x08A05400)
#define gFollowingMonPic_0359_AltariaTiles ((u8*)0x08A06000)
#define gFollowingMonPic_0360_WynautTiles ((u8*)0x08A06C00)
#define gFollowingMonPic_0361_DuskullTiles ((u8*)0x08A07800)
#define gFollowingMonPic_0362_DusclopsTiles ((u8*)0x08A08400)
#define gFollowingMonPic_0363_RoseliaTiles ((u8*)0x08A09000)
#define gFollowingMonPic_0364_SlakothTiles ((u8*)0x08A09C00)
#define gFollowingMonPic_0365_VigorothTiles ((u8*)0x08A0A800)
#define gFollowingMonPic_0366_SlakingTiles ((u8*)0x08A0B400)
#define gFollowingMonPic_0367_GulpinTiles ((u8*)0x08A0C000)
#define gFollowingMonPic_0368_SwalotTiles ((u8*)0x08A0CC00)
#define gFollowingMonPic_0369_TropiusTiles ((u8*)0x08A0D800)
#define gFollowingMonPic_0370_WhismurTiles ((u8*)0x08A0E400)
#define gFollowingMonPic_0371_LoudredTiles ((u8*)0x08A0F000)
#define gFollowingMonPic_0372_ExploudTiles ((u8*)0x08A0FC00)
#define gFollowingMonPic_0373_ClamperlTiles ((u8*)0x08A10800)
#define gFollowingMonPic_0374_HuntailTiles ((u8*)0x08A11400)
#define gFollowingMonPic_0375_GorebyssTiles ((u8*)0x08A12000)
#define gFollowingMonPic_0376_AbsolTiles ((u8*)0x08A12C00)
#define gFollowingMonPic_0377_ShuppetTiles ((u8*)0x08A13800)
#define gFollowingMonPic_0378_BanetteTiles ((u8*)0x08A14400)
#define gFollowingMonPic_0379_SeviperTiles ((u8*)0x08A15000)
#define gFollowingMonPic_0380_ZangooseTiles ((u8*)0x08A15C00)
#define gFollowingMonPic_0381_RelicanthTiles ((u8*)0x08A16800)
#define gFollowingMonPic_0382_AronTiles ((u8*)0x08A17400)
#define gFollowingMonPic_0383_LaironTiles ((u8*)0x08A18000)
#define gFollowingMonPic_0384_AggronTiles ((u8*)0x08A18C00)
#define gFollowingMonPic_0385_CastformTiles ((u8*)0x08A19800)
#define gFollowingMonPic_0386_VolbeatTiles ((u8*)0x08A1A400)
#define gFollowingMonPic_0387_IllumiseTiles ((u8*)0x08A1B000)
#define gFollowingMonPic_0388_LileepTiles ((u8*)0x08A1BC00)
#define gFollowingMonPic_0389_CradilyTiles ((u8*)0x08A1C800)
#define gFollowingMonPic_0390_AnorithTiles ((u8*)0x08A1D400)
#define gFollowingMonPic_0391_ArmaldoTiles ((u8*)0x08A1E000)
#define gFollowingMonPic_0392_RaltsTiles ((u8*)0x08A1EC00)
#define gFollowingMonPic_0393_KirliaTiles ((u8*)0x08A1F800)
#define gFollowingMonPic_0394_GardevoirTiles ((u8*)0x08A20400)
#define gFollowingMonPic_0395_BagonTiles ((u8*)0x08A21000)
#define gFollowingMonPic_0396_ShelgonTiles ((u8*)0x08A21C00)
#define gFollowingMonPic_0397_SalamenceTiles ((u8*)0x08A22800)
#define gFollowingMonPic_0398_BeldumTiles ((u8*)0x08A23400)
#define gFollowingMonPic_0399_MetangTiles ((u8*)0x08A24000)
#define gFollowingMonPic_0400_MetagrossTiles ((u8*)0x08A24C00)
#define gFollowingMonPic_0401_RegirockTiles ((u8*)0x08A25800)
#define gFollowingMonPic_0402_RegiceTiles ((u8*)0x08A26400)
#define gFollowingMonPic_0403_RegisteelTiles ((u8*)0x08A27000)
#define gFollowingMonPic_0404_KyogreTiles ((u8*)0x08A27C00)
#define gFollowingMonPic_0405_GroudonTiles ((u8*)0x08A2AC00)
#define gFollowingMonPic_0406_RayquazaTiles ((u8*)0x08A2DC00)
#define gFollowingMonPic_0407_LatiasTiles ((u8*)0x08A30C00)
#define gFollowingMonPic_0408_LatiosTiles ((u8*)0x08A31800)
#define gFollowingMonPic_0409_JirachiTiles ((u8*)0x08A32400)
#define gFollowingMonPic_0410_DeoxysTiles ((u8*)0x08A33000)
#define gFollowingMonPic_0411_ChimechoTiles ((u8*)0x08A33C00)
#define gFollowingMonPic_0440_TurtwigTiles ((u8*)0x08A34800)
#define gFollowingMonPic_0441_GrotleTiles ((u8*)0x08A35400)
#define gFollowingMonPic_0442_TorterraTiles ((u8*)0x08A36000)
#define gFollowingMonPic_0443_ChimcharTiles ((u8*)0x08A36C00)
#define gFollowingMonPic_0444_MonfernoTiles ((u8*)0x08A37800)
#define gFollowingMonPic_0445_InfernapeTiles ((u8*)0x08A38400)
#define gFollowingMonPic_0446_PiplupTiles ((u8*)0x08A39000)
#define gFollowingMonPic_0447_PrinplupTiles ((u8*)0x08A39C00)
#define gFollowingMonPic_0448_EmpoleonTiles ((u8*)0x08A3A800)
#define gFollowingMonPic_0449_StarlyTiles ((u8*)0x08A3B400)
#define gFollowingMonPic_0450_StaraviaTiles ((u8*)0x08A3C000)
#define gFollowingMonPic_0451_StaraptorTiles ((u8*)0x08A3CC00)
#define gFollowingMonPic_0452_BidoofTiles ((u8*)0x08A3D800)
#define gFollowingMonPic_0453_BibarelTiles ((u8*)0x08A3E400)
#define gFollowingMonPic_0454_KricketotTiles ((u8*)0x08A3F000)
#define gFollowingMonPic_0455_KricketuneTiles ((u8*)0x08A3FC00)
#define gFollowingMonPic_0456_ShinxTiles ((u8*)0x08A40800)
#define gFollowingMonPic_0457_LuxioTiles ((u8*)0x08A41400)
#define gFollowingMonPic_0458_LuxrayTiles ((u8*)0x08A42000)
#define gFollowingMonPic_0459_BudewTiles ((u8*)0x08A42C00)
#define gFollowingMonPic_0460_RoseradeTiles ((u8*)0x08A43800)
#define gFollowingMonPic_0461_CranidosTiles ((u8*)0x08A44400)
#define gFollowingMonPic_0462_RampardosTiles ((u8*)0x08A45000)
#define gFollowingMonPic_0463_ShieldonTiles ((u8*)0x08A45C00)
#define gFollowingMonPic_0464_BastiodonTiles ((u8*)0x08A46800)
#define gFollowingMonPic_0465_BurmyTiles ((u8*)0x08A47400)
#define gFollowingMonPic_0466_WormadamTiles ((u8*)0x08A48000)
#define gFollowingMonPic_0467_MothimTiles ((u8*)0x08A48C00)
#define gFollowingMonPic_0468_CombeeTiles ((u8*)0x08A49800)
#define gFollowingMonPic_0469_VespiquenTiles ((u8*)0x08A4A400)
#define gFollowingMonPic_0470_PachirisuTiles ((u8*)0x08A4B000)
#define gFollowingMonPic_0471_BuizelTiles ((u8*)0x08A4BC00)
#define gFollowingMonPic_0472_FloatzelTiles ((u8*)0x08A4C800)
#define gFollowingMonPic_0473_CherubiTiles ((u8*)0x08A4D400)
#define gFollowingMonPic_0474_CherrimTiles ((u8*)0x08A4E000)
#define gFollowingMonPic_0475_ShellosTiles ((u8*)0x08A4EC00)
#define gFollowingMonPic_0476_GastrodonTiles ((u8*)0x08A4F800)
#define gFollowingMonPic_0477_AmbipomTiles ((u8*)0x08A50400)
#define gFollowingMonPic_0478_DrifloonTiles ((u8*)0x08A51000)
#define gFollowingMonPic_0479_DrifblimTiles ((u8*)0x08A51C00)
#define gFollowingMonPic_0480_BunearyTiles ((u8*)0x08A52800)
#define gFollowingMonPic_0481_LopunnyTiles ((u8*)0x08A53400)
#define gFollowingMonPic_0482_MismagiusTiles ((u8*)0x08A54000)
#define gFollowingMonPic_0483_HonchkrowTiles ((u8*)0x08A54C00)
#define gFollowingMonPic_0484_GlameowTiles ((u8*)0x08A55800)
#define gFollowingMonPic_0485_PuruglyTiles ((u8*)0x08A56400)
#define gFollowingMonPic_0486_ChinglingTiles ((u8*)0x08A57000)
#define gFollowingMonPic_0487_StunkyTiles ((u8*)0x08A57C00)
#define gFollowingMonPic_0488_SkuntankTiles ((u8*)0x08A58800)
#define gFollowingMonPic_0489_BronzorTiles ((u8*)0x08A59400)
#define gFollowingMonPic_0490_BronzongTiles ((u8*)0x08A5A000)
#define gFollowingMonPic_0491_BonslyTiles ((u8*)0x08A5AC00)
#define gFollowingMonPic_0492_Mime_JrTiles ((u8*)0x08A5B800)
#define gFollowingMonPic_0493_HappinyTiles ((u8*)0x08A5C400)
#define gFollowingMonPic_0494_ChatotTiles ((u8*)0x08A5D000)
#define gFollowingMonPic_0495_SpiritombTiles ((u8*)0x08A5DC00)
#define gFollowingMonPic_0496_GibleTiles ((u8*)0x08A5E800)
#define gFollowingMonPic_0497_GabiteTiles ((u8*)0x08A5F400)
#define gFollowingMonPic_0498_GarchompTiles ((u8*)0x08A60000)
#define gFollowingMonPic_0499_MunchlaxTiles ((u8*)0x08A60C00)
#define gFollowingMonPic_0500_RioluTiles ((u8*)0x08A61800)
#define gFollowingMonPic_0501_LucarioTiles ((u8*)0x08A62400)
#define gFollowingMonPic_0502_HippopotasTiles ((u8*)0x08A63000)
#define gFollowingMonPic_0503_HippowdonTiles ((u8*)0x08A63C00)
#define gFollowingMonPic_0504_SkorupiTiles ((u8*)0x08A64800)
#define gFollowingMonPic_0505_DrapionTiles ((u8*)0x08A65400)
#define gFollowingMonPic_0506_CroagunkTiles ((u8*)0x08A66000)
#define gFollowingMonPic_0507_ToxicroakTiles ((u8*)0x08A66C00)
#define gFollowingMonPic_0508_CarnivineTiles ((u8*)0x08A67800)
#define gFollowingMonPic_0509_FinneonTiles ((u8*)0x08A68400)
#define gFollowingMonPic_0510_LumineonTiles ((u8*)0x08A69000)
#define gFollowingMonPic_0511_MantykeTiles ((u8*)0x08A69C00)
#define gFollowingMonPic_0512_SnoverTiles ((u8*)0x08A6A800)
#define gFollowingMonPic_0513_AbomasnowTiles ((u8*)0x08A6B400)
#define gFollowingMonPic_0514_WeavileTiles ((u8*)0x08A6C000)
#define gFollowingMonPic_0515_MagnezoneTiles ((u8*)0x08A6CC00)
#define gFollowingMonPic_0516_LickilickyTiles ((u8*)0x08A6D800)
#define gFollowingMonPic_0517_RhyperiorTiles ((u8*)0x08A6E400)
#define gFollowingMonPic_0518_TangrowthTiles ((u8*)0x08A6F000)
#define gFollowingMonPic_0519_ElectivireTiles ((u8*)0x08A6FC00)
#define gFollowingMonPic_0520_MagmortarTiles ((u8*)0x08A70800)
#define gFollowingMonPic_0521_TogekissTiles ((u8*)0x08A71400)
#define gFollowingMonPic_0522_YanmegaTiles ((u8*)0x08A72000)
#define gFollowingMonPic_0523_LeafeonTiles ((u8*)0x08A72C00)
#define gFollowingMonPic_0524_GlaceonTiles ((u8*)0x08A73800)
#define gFollowingMonPic_0525_GliscorTiles ((u8*)0x08A74400)
#define gFollowingMonPic_0526_MamoswineTiles ((u8*)0x08A75000)
#define gFollowingMonPic_0527_Porygon_ZTiles ((u8*)0x08A75C00)
#define gFollowingMonPic_0528_GalladeTiles ((u8*)0x08A76800)
#define gFollowingMonPic_0529_ProbopassTiles ((u8*)0x08A77400)
#define gFollowingMonPic_0530_DusknoirTiles ((u8*)0x08A78000)
#define gFollowingMonPic_0531_FroslassTiles ((u8*)0x08A78C00)
#define gFollowingMonPic_0532_RotomTiles ((u8*)0x08A79800)
#define gFollowingMonPic_0533_UxieTiles ((u8*)0x08A7A400)
#define gFollowingMonPic_0534_MespritTiles ((u8*)0x08A7B000)
#define gFollowingMonPic_0535_AzelfTiles ((u8*)0x08A7BC00)
#define gFollowingMonPic_0536_DialgaTiles ((u8*)0x08A7C800)
#define gFollowingMonPic_0537_PalkiaTiles ((u8*)0x08A7F800)
#define gFollowingMonPic_0538_HeatranTiles ((u8*)0x08A82800)
#define gFollowingMonPic_0539_RegigigasTiles ((u8*)0x08A83400)
#define gFollowingMonPic_0540_GiratinaTiles ((u8*)0x08A86400)
#define gFollowingMonPic_0541_CresseliaTiles ((u8*)0x08A89400)
#define gFollowingMonPic_0542_PhioneTiles ((u8*)0x08A8A000)
#define gFollowingMonPic_0543_ManaphyTiles ((u8*)0x08A8AC00)
#define gFollowingMonPic_0544_DarkraiTiles ((u8*)0x08A8B800)
#define gFollowingMonPic_0545_ShayminTiles ((u8*)0x08A8C400)
#define gFollowingMonPic_0546_ArceusTiles ((u8*)0x08A8D000)
#define gFollowingMonPic_0547_VictiniTiles ((u8*)0x08A90000)
#define gFollowingMonPic_0548_SnivyTiles ((u8*)0x08A90C00)
#define gFollowingMonPic_0549_ServineTiles ((u8*)0x08A91800)
#define gFollowingMonPic_0550_SerperiorTiles ((u8*)0x08A92400)
#define gFollowingMonPic_0551_TepigTiles ((u8*)0x08A93000)
#define gFollowingMonPic_0552_PigniteTiles ((u8*)0x08A93C00)
#define gFollowingMonPic_0553_EmboarTiles ((u8*)0x08A94800)
#define gFollowingMonPic_0554_OshawottTiles ((u8*)0x08A95400)
#define gFollowingMonPic_0555_DewottTiles ((u8*)0x08A96000)
#define gFollowingMonPic_0556_SamurottTiles ((u8*)0x08A96C00)
#define gFollowingMonPic_0556_Samurott_1Tiles ((u8*)0x08A97800)
#define gFollowingMonPic_0557_PatratTiles ((u8*)0x08A98400)
#define gFollowingMonPic_0558_WatchogTiles ((u8*)0x08A99000)
#define gFollowingMonPic_0559_LillipupTiles ((u8*)0x08A99C00)
#define gFollowingMonPic_0560_HerdierTiles ((u8*)0x08A9A800)
#define gFollowingMonPic_0561_StoutlandTiles ((u8*)0x08A9B400)
#define gFollowingMonPic_0562_PurrloinTiles ((u8*)0x08A9C000)
#define gFollowingMonPic_0563_LiepardTiles ((u8*)0x08A9CC00)
#define gFollowingMonPic_0564_PansageTiles ((u8*)0x08A9D800)
#define gFollowingMonPic_0565_SimisageTiles ((u8*)0x08A9E400)
#define gFollowingMonPic_0566_PansearTiles ((u8*)0x08A9F000)
#define gFollowingMonPic_0567_SimisearTiles ((u8*)0x08A9FC00)
#define gFollowingMonPic_0568_PanpourTiles ((u8*)0x08AA0800)
#define gFollowingMonPic_0569_SimipourTiles ((u8*)0x08AA1400)
#define gFollowingMonPic_0570_MunnaTiles ((u8*)0x08AA2000)
#define gFollowingMonPic_0571_MusharnaTiles ((u8*)0x08AA2C00)
#define gFollowingMonPic_0572_PidoveTiles ((u8*)0x08AA3800)
#define gFollowingMonPic_0573_TranquillTiles ((u8*)0x08AA4400)
#define gFollowingMonPic_0574_UnfezantTiles ((u8*)0x08AA5000)
#define gFollowingMonPic_0575_BlitzleTiles ((u8*)0x08AA5C00)
#define gFollowingMonPic_0576_ZebstrikaTiles ((u8*)0x08AA6800)
#define gFollowingMonPic_0577_RoggenrolaTiles ((u8*)0x08AA7400)
#define gFollowingMonPic_0578_BoldoreTiles ((u8*)0x08AA8000)
#define gFollowingMonPic_0579_GigalithTiles ((u8*)0x08AA8C00)
#define gFollowingMonPic_0580_WoobatTiles ((u8*)0x08AA9800)
#define gFollowingMonPic_0581_SwoobatTiles ((u8*)0x08AAA400)
#define gFollowingMonPic_0582_DrilburTiles ((u8*)0x08AAB000)
#define gFollowingMonPic_0583_ExcadrillTiles ((u8*)0x08AABC00)
#define gFollowingMonPic_0584_AudinoTiles ((u8*)0x08AAC800)
#define gFollowingMonPic_0585_TimburrTiles ((u8*)0x08AAD400)
#define gFollowingMonPic_0586_GurdurrTiles ((u8*)0x08AAE000)
#define gFollowingMonPic_0587_ConkeldurrTiles ((u8*)0x08AAEC00)
#define gFollowingMonPic_0588_TympoleTiles ((u8*)0x08AAF800)
#define gFollowingMonPic_0589_PalpitoadTiles ((u8*)0x08AB0400)
#define gFollowingMonPic_0590_SeismitoadTiles ((u8*)0x08AB1000)
#define gFollowingMonPic_0591_ThrohTiles ((u8*)0x08AB1C00)
#define gFollowingMonPic_0592_SawkTiles ((u8*)0x08AB2800)
#define gFollowingMonPic_0593_SewaddleTiles ((u8*)0x08AB3400)
#define gFollowingMonPic_0594_SwadloonTiles ((u8*)0x08AB4000)
#define gFollowingMonPic_0595_LeavannyTiles ((u8*)0x08AB4C00)
#define gFollowingMonPic_0596_VenipedeTiles ((u8*)0x08AB5800)
#define gFollowingMonPic_0597_WhirlipedeTiles ((u8*)0x08AB6400)
#define gFollowingMonPic_0598_ScolipedeTiles ((u8*)0x08AB7000)
#define gFollowingMonPic_0599_CottoneeTiles ((u8*)0x08AB7C00)
#define gFollowingMonPic_0600_WhimsicottTiles ((u8*)0x08AB8800)
#define gFollowingMonPic_0601_PetililTiles ((u8*)0x08AB9400)
#define gFollowingMonPic_0602_LilligantTiles ((u8*)0x08ABA000)
#define gFollowingMonPic_0603_BasculinTiles ((u8*)0x08ABAC00)
#define gFollowingMonPic_0604_SandileTiles ((u8*)0x08ABB800)
#define gFollowingMonPic_0605_KrokorokTiles ((u8*)0x08ABC400)
#define gFollowingMonPic_0606_KrookodileTiles ((u8*)0x08ABD000)
#define gFollowingMonPic_0607_DarumakaTiles ((u8*)0x08ABDC00)
#define gFollowingMonPic_0608_DarmanitanTiles ((u8*)0x08ABE800)
#define gFollowingMonPic_0609_MaractusTiles ((u8*)0x08ABF400)
#define gFollowingMonPic_0610_DwebbleTiles ((u8*)0x08AC0000)
#define gFollowingMonPic_0611_CrustleTiles ((u8*)0x08AC0C00)
#define gFollowingMonPic_0612_ScraggyTiles ((u8*)0x08AC1800)
#define gFollowingMonPic_0613_ScraftyTiles ((u8*)0x08AC2400)
#define gFollowingMonPic_0614_SigilyphTiles ((u8*)0x08AC3000)
#define gFollowingMonPic_0615_YamaskTiles ((u8*)0x08AC3C00)
#define gFollowingMonPic_0616_CofagrigusTiles ((u8*)0x08AC4800)
#define gFollowingMonPic_0617_TirtougaTiles ((u8*)0x08AC5400)
#define gFollowingMonPic_0618_CarracostaTiles ((u8*)0x08AC6000)
#define gFollowingMonPic_0619_ArchenTiles ((u8*)0x08AC6C00)
#define gFollowingMonPic_0620_ArcheopsTiles ((u8*)0x08AC7800)
#define gFollowingMonPic_0621_TrubbishTiles ((u8*)0x08AC8400)
#define gFollowingMonPic_0622_GarbodorTiles ((u8*)0x08AC9000)
#define gFollowingMonPic_0623_ZoruaTiles ((u8*)0x08AC9C00)
#define gFollowingMonPic_0624_ZoroarkTiles ((u8*)0x08ACA800)
#define gFollowingMonPic_0625_MinccinoTiles ((u8*)0x08ACB400)
#define gFollowingMonPic_0626_CinccinoTiles ((u8*)0x08ACC000)
#define gFollowingMonPic_0627_GothitaTiles ((u8*)0x08ACCC00)
#define gFollowingMonPic_0628_GothoritaTiles ((u8*)0x08ACD800)
#define gFollowingMonPic_0629_GothitelleTiles ((u8*)0x08ACE400)
#define gFollowingMonPic_0630_SolosisTiles ((u8*)0x08ACF000)
#define gFollowingMonPic_0631_DuosionTiles ((u8*)0x08ACFC00)
#define gFollowingMonPic_0632_ReuniclusTiles ((u8*)0x08AD0800)
#define gFollowingMonPic_0633_DucklettTiles ((u8*)0x08AD1400)
#define gFollowingMonPic_0634_SwannaTiles ((u8*)0x08AD2000)
#define gFollowingMonPic_0635_VanilliteTiles ((u8*)0x08AD2C00)
#define gFollowingMonPic_0636_VanillishTiles ((u8*)0x08AD3800)
#define gFollowingMonPic_0637_VanilluxeTiles ((u8*)0x08AD4400)
#define gFollowingMonPic_0638_DeerlingTiles ((u8*)0x08AD5000)
#define gFollowingMonPic_0639_SawsbuckTiles ((u8*)0x08AD5C00)
#define gFollowingMonPic_0640_EmolgaTiles ((u8*)0x08AD6800)
#define gFollowingMonPic_0641_KarrablastTiles ((u8*)0x08AD7400)
#define gFollowingMonPic_0642_EscavalierTiles ((u8*)0x08AD8000)
#define gFollowingMonPic_0643_FoongusTiles ((u8*)0x08AD8C00)
#define gFollowingMonPic_0644_AmoongussTiles ((u8*)0x08AD9800)
#define gFollowingMonPic_0645_FrillishTiles ((u8*)0x08ADA400)
#define gFollowingMonPic_0645_Frillish_FTiles ((u8*)0x08ADB000)
#define gFollowingMonPic_0646_JellicentTiles ((u8*)0x08ADBC00)
#define gFollowingMonPic_0646_Jellicent_FTiles ((u8*)0x08ADC800)
#define gFollowingMonPic_0647_AlomomolaTiles ((u8*)0x08ADD400)
#define gFollowingMonPic_0648_JoltikTiles ((u8*)0x08ADE000)
#define gFollowingMonPic_0649_GalvantulaTiles ((u8*)0x08ADEC00)
#define gFollowingMonPic_0650_FerroseedTiles ((u8*)0x08ADF800)
#define gFollowingMonPic_0651_FerrothornTiles ((u8*)0x08AE0400)
#define gFollowingMonPic_0652_KlinkTiles ((u8*)0x08AE1000)
#define gFollowingMonPic_0653_KlangTiles ((u8*)0x08AE1C00)
#define gFollowingMonPic_0654_KlinklangTiles ((u8*)0x08AE2800)
#define gFollowingMonPic_0655_TynamoTiles ((u8*)0x08AE3400)
#define gFollowingMonPic_0656_EelektrikTiles ((u8*)0x08AE4000)
#define gFollowingMonPic_0657_EelektrossTiles ((u8*)0x08AE4C00)
#define gFollowingMonPic_0658_ElgyemTiles ((u8*)0x08AE5800)
#define gFollowingMonPic_0659_BeheeyemTiles ((u8*)0x08AE6400)
#define gFollowingMonPic_0660_LitwickTiles ((u8*)0x08AE7000)
#define gFollowingMonPic_0661_LampentTiles ((u8*)0x08AE7C00)
#define gFollowingMonPic_0662_ChandelureTiles ((u8*)0x08AE8800)
#define gFollowingMonPic_0663_AxewTiles ((u8*)0x08AE9400)
#define gFollowingMonPic_0664_FraxureTiles ((u8*)0x08AEA000)
#define gFollowingMonPic_0665_HaxorusTiles ((u8*)0x08AEAC00)
#define gFollowingMonPic_0666_CubchooTiles ((u8*)0x08AEB800)
#define gFollowingMonPic_0667_BearticTiles ((u8*)0x08AEC400)
#define gFollowingMonPic_0668_CryogonalTiles ((u8*)0x08AED000)
#define gFollowingMonPic_0669_ShelmetTiles ((u8*)0x08AEDC00)
#define gFollowingMonPic_0670_AccelgorTiles ((u8*)0x08AEE800)
#define gFollowingMonPic_0671_StunfiskTiles ((u8*)0x08AEF400)
#define gFollowingMonPic_0672_MienfooTiles ((u8*)0x08AF0000)
#define gFollowingMonPic_0673_MienshaoTiles ((u8*)0x08AF0C00)
#define gFollowingMonPic_0674_DruddigonTiles ((u8*)0x08AF1800)
#define gFollowingMonPic_0675_GolettTiles ((u8*)0x08AF2400)
#define gFollowingMonPic_0676_GolurkTiles ((u8*)0x08AF3000)
#define gFollowingMonPic_0677_PawniardTiles ((u8*)0x08AF3C00)
#define gFollowingMonPic_0678_BisharpTiles ((u8*)0x08AF4800)
#define gFollowingMonPic_0679_BouffalantTiles ((u8*)0x08AF5400)
#define gFollowingMonPic_0680_RuffletTiles ((u8*)0x08AF6000)
#define gFollowingMonPic_0681_BraviaryTiles ((u8*)0x08AF6C00)
#define gFollowingMonPic_0682_VullabyTiles ((u8*)0x08AF7800)
#define gFollowingMonPic_0683_MandibuzzTiles ((u8*)0x08AF8400)
#define gFollowingMonPic_0684_HeatmorTiles ((u8*)0x08AF9000)
#define gFollowingMonPic_0685_DurantTiles ((u8*)0x08AF9C00)
#define gFollowingMonPic_0686_DeinoTiles ((u8*)0x08AFA800)
#define gFollowingMonPic_0687_ZweilousTiles ((u8*)0x08AFB400)
#define gFollowingMonPic_0688_HydreigonTiles ((u8*)0x08AFC000)
#define gFollowingMonPic_0689_LarvestaTiles ((u8*)0x08AFCC00)
#define gFollowingMonPic_0690_VolcaronaTiles ((u8*)0x08AFD800)
#define gFollowingMonPic_0691_CobalionTiles ((u8*)0x08AFE400)
#define gFollowingMonPic_0692_TerrakionTiles ((u8*)0x08AFF000)
#define gFollowingMonPic_0693_VirizionTiles ((u8*)0x08AFFC00)
#define gFollowingMonPic_0694_TornadusTiles ((u8*)0x08B00800)
#define gFollowingMonPic_0695_ThundurusTiles ((u8*)0x08B01400)
#define gFollowingMonPic_0696_ReshiramTiles ((u8*)0x08B02000)
#define gFollowingMonPic_0697_ZekromTiles ((u8*)0x08B05000)
#define gFollowingMonPic_0698_LandorusTiles ((u8*)0x08B08000)
#define gFollowingMonPic_0699_KyuremTiles ((u8*)0x08B08C00)
#define gFollowingMonPic_0700_KeldeoTiles ((u8*)0x08B0BC00)
#define gFollowingMonPic_0701_MeloettaTiles ((u8*)0x08B0C800)
#define gFollowingMonPic_0702_GenesectTiles ((u8*)0x08B0D400)
#define gFollowingMonPic_0703_Unfezant_FTiles ((u8*)0x08B0E000)
#define gFollowingMonPic_0711_Shellos_ETiles ((u8*)0x08B0EC00)
#define gFollowingMonPic_0712_Gastrodon_ETiles ((u8*)0x08B0F800)
#define gFollowingMonPic_0713_Rotom_HTiles ((u8*)0x08B10400)
#define gFollowingMonPic_0714_Rotom_WTiles ((u8*)0x08B11000)
#define gFollowingMonPic_0715_Rotom_FRTiles ((u8*)0x08B11C00)
#define gFollowingMonPic_0716_Rotom_FTiles ((u8*)0x08B12800)
#define gFollowingMonPic_0717_Rotom_MTiles ((u8*)0x08B13400)
#define gFollowingMonPic_0718_Giratina_OTiles ((u8*)0x08B14000)
#define gFollowingMonPic_0719_Shaymin_STiles ((u8*)0x08B17000)
#define gFollowingMonPic_0736_Basculin_BTiles ((u8*)0x08B17C00)
#define gFollowingMonPic_0746_Meloetta_PTiles ((u8*)0x08B18800)
#define gFollowingMonPic_0752_Kyurem_BTiles ((u8*)0x08B19400)
#define gFollowingMonPic_0753_Kyurem_WTiles ((u8*)0x08B1A000)
#define gFollowingMonPic_0754_Tornadus_TTiles ((u8*)0x08B1AC00)
#define gFollowingMonPic_0755_Thundurus_TTiles ((u8*)0x08B1DC00)
#define gFollowingMonPic_0756_Landorus_TTiles ((u8*)0x08B20C00)
#define gFollowingMonPic_0758_ChespinTiles ((u8*)0x08B21800)
#define gFollowingMonPic_0759_QuilladinTiles ((u8*)0x08B22400)
#define gFollowingMonPic_0760_ChesnaughtTiles ((u8*)0x08B23000)
#define gFollowingMonPic_0761_FennekinTiles ((u8*)0x08B23C00)
#define gFollowingMonPic_0762_BraixenTiles ((u8*)0x08B24800)
#define gFollowingMonPic_0763_DelphoxTiles ((u8*)0x08B25400)
#define gFollowingMonPic_0764_FroakieTiles ((u8*)0x08B26000)
#define gFollowingMonPic_0765_FrogadierTiles ((u8*)0x08B26C00)
#define gFollowingMonPic_0766_GreninjaTiles ((u8*)0x08B27800)
#define gFollowingMonPic_0767_BunnelbyTiles ((u8*)0x08B28400)
#define gFollowingMonPic_0768_DiggersbyTiles ((u8*)0x08B29000)
#define gFollowingMonPic_0769_FletchlingTiles ((u8*)0x08B29C00)
#define gFollowingMonPic_0770_FletchinderTiles ((u8*)0x08B2A800)
#define gFollowingMonPic_0771_TalonflameTiles ((u8*)0x08B2B400)
#define gFollowingMonPic_0772_ScatterbugTiles ((u8*)0x08B2C000)
#define gFollowingMonPic_0773_SpewpaTiles ((u8*)0x08B2CC00)
#define gFollowingMonPic_0774_VivillonTiles ((u8*)0x08B2D800)
#define gFollowingMonPic_0775_LitleoTiles ((u8*)0x08B2E400)
#define gFollowingMonPic_0776_PyroarTiles ((u8*)0x08B2F000)
#define gFollowingMonPic_0777_FlabebeTiles ((u8*)0x08B2FC00)
#define gFollowingMonPic_0778_FloetteTiles ((u8*)0x08B30800)
#define gFollowingMonPic_0779_FlorgesTiles ((u8*)0x08B31400)
#define gFollowingMonPic_0780_SkiddoTiles ((u8*)0x08B32000)
#define gFollowingMonPic_0781_GogoatTiles ((u8*)0x08B32C00)
#define gFollowingMonPic_0782_PanchamTiles ((u8*)0x08B33800)
#define gFollowingMonPic_0783_PangoroTiles ((u8*)0x08B34400)
#define gFollowingMonPic_0784_FurfrouTiles ((u8*)0x08B35000)
#define gFollowingMonPic_0785_EspurrTiles ((u8*)0x08B35C00)
#define gFollowingMonPic_0786_MeowsticTiles ((u8*)0x08B36800)
#define gFollowingMonPic_0787_HonedgeTiles ((u8*)0x08B37400)
#define gFollowingMonPic_0788_DoubladeTiles ((u8*)0x08B38000)
#define gFollowingMonPic_0789_AegislashTiles ((u8*)0x08B38C00)
#define gFollowingMonPic_0790_SpritzeeTiles ((u8*)0x08B39800)
#define gFollowingMonPic_0791_AromatisseTiles ((u8*)0x08B3A400)
#define gFollowingMonPic_0792_SwirlixTiles ((u8*)0x08B3B000)
#define gFollowingMonPic_0793_SlurpuffTiles ((u8*)0x08B3BC00)
#define gFollowingMonPic_0794_InkayTiles ((u8*)0x08B3C800)
#define gFollowingMonPic_0795_MalamarTiles ((u8*)0x08B3D400)
#define gFollowingMonPic_0796_BinacleTiles ((u8*)0x08B3E000)
#define gFollowingMonPic_0797_BarbaracleTiles ((u8*)0x08B3EC00)
#define gFollowingMonPic_0798_SkrelpTiles ((u8*)0x08B3F800)
#define gFollowingMonPic_0799_DragalgeTiles ((u8*)0x08B40400)
#define gFollowingMonPic_0800_ClauncherTiles ((u8*)0x08B41000)
#define gFollowingMonPic_0801_ClawitzerTiles ((u8*)0x08B41C00)
#define gFollowingMonPic_0802_HelioptileTiles ((u8*)0x08B42800)
#define gFollowingMonPic_0803_HelioliskTiles ((u8*)0x08B43400)
#define gFollowingMonPic_0804_TyruntTiles ((u8*)0x08B44000)
#define gFollowingMonPic_0805_TyrantrumTiles ((u8*)0x08B44C00)
#define gFollowingMonPic_0806_AmauraTiles ((u8*)0x08B45800)
#define gFollowingMonPic_0807_AurorusTiles ((u8*)0x08B46400)
#define gFollowingMonPic_0808_SylveonTiles ((u8*)0x08B47000)
#define gFollowingMonPic_0809_HawluchaTiles ((u8*)0x08B47C00)
#define gFollowingMonPic_0810_DedenneTiles ((u8*)0x08B48800)
#define gFollowingMonPic_0811_CarbinkTiles ((u8*)0x08B49400)
#define gFollowingMonPic_0812_GoomyTiles ((u8*)0x08B4A000)
#define gFollowingMonPic_0813_SliggooTiles ((u8*)0x08B4AC00)
#define gFollowingMonPic_0814_GoodraTiles ((u8*)0x08B4B800)
#define gFollowingMonPic_0815_KlefkiTiles ((u8*)0x08B4C400)
#define gFollowingMonPic_0816_PhantumpTiles ((u8*)0x08B4D000)
#define gFollowingMonPic_0817_TrevenantTiles ((u8*)0x08B4DC00)
#define gFollowingMonPic_0818_PumpkabooTiles ((u8*)0x08B4E800)
#define gFollowingMonPic_0819_GourgeistTiles ((u8*)0x08B4F400)
#define gFollowingMonPic_0820_BergmiteTiles ((u8*)0x08B50000)
#define gFollowingMonPic_0821_AvaluggTiles ((u8*)0x08B50C00)
#define gFollowingMonPic_0822_NoibatTiles ((u8*)0x08B51800)
#define gFollowingMonPic_0823_NoivernTiles ((u8*)0x08B52400)
#define gFollowingMonPic_0824_XerneasTiles ((u8*)0x08B53000)
#define gFollowingMonPic_0825_YveltalTiles ((u8*)0x08B56000)
#define gFollowingMonPic_0826_ZygardeTiles ((u8*)0x08B59000)
#define gFollowingMonPic_0827_DiancieTiles ((u8*)0x08B5C000)
#define gFollowingMonPic_0828_HoopaTiles ((u8*)0x08B5CC00)
#define gFollowingMonPic_0829_Hoopa_UTiles ((u8*)0x08B5D800)
#define gFollowingMonPic_0830_VolcanionTiles ((u8*)0x08B60800)
#define gFollowingMonPic_0831_Pyroar_FTiles ((u8*)0x08B61400)
#define gFollowingMonPic_0832_Meowstic_FTiles ((u8*)0x08B62000)
#define gFollowingMonPic_0837_Zygarde_10Tiles ((u8*)0x08B62C00)
#define gFollowingMonPic_0848_Floette_ETiles ((u8*)0x08B63800)
#define gFollowingMonPic_0939_RowletTiles ((u8*)0x08B64400)
#define gFollowingMonPic_0940_DartrixTiles ((u8*)0x08B65000)
#define gFollowingMonPic_0941_DecidueyeTiles ((u8*)0x08B65C00)
#define gFollowingMonPic_0942_LittenTiles ((u8*)0x08B66800)
#define gFollowingMonPic_0943_TorracatTiles ((u8*)0x08B67400)
#define gFollowingMonPic_0944_IncineroarTiles ((u8*)0x08B68000)
#define gFollowingMonPic_0945_PopplioTiles ((u8*)0x08B68C00)
#define gFollowingMonPic_0946_BrionneTiles ((u8*)0x08B69800)
#define gFollowingMonPic_0947_PrimarinaTiles ((u8*)0x08B6A400)
#define gFollowingMonPic_0948_PikipekTiles ((u8*)0x08B6B000)
#define gFollowingMonPic_0949_TrumbeakTiles ((u8*)0x08B6BC00)
#define gFollowingMonPic_0950_ToucannonTiles ((u8*)0x08B6C800)
#define gFollowingMonPic_0951_YungoosTiles ((u8*)0x08B6D400)
#define gFollowingMonPic_0952_GumshoosTiles ((u8*)0x08B6E000)
#define gFollowingMonPic_0953_GrubbinTiles ((u8*)0x08B6EC00)
#define gFollowingMonPic_0954_CharjabugTiles ((u8*)0x08B6F800)
#define gFollowingMonPic_0955_VikavoltTiles ((u8*)0x08B70400)
#define gFollowingMonPic_0956_CrabrawlerTiles ((u8*)0x08B71000)
#define gFollowingMonPic_0957_CrabominableTiles ((u8*)0x08B71C00)
#define gFollowingMonPic_0958_OricorioTiles ((u8*)0x08B72800)
#define gFollowingMonPic_0959_CutieflyTiles ((u8*)0x08B73400)
#define gFollowingMonPic_0960_RibombeeTiles ((u8*)0x08B74000)
#define gFollowingMonPic_0961_RockruffTiles ((u8*)0x08B74C00)
#define gFollowingMonPic_0962_LycanrocTiles ((u8*)0x08B75800)
#define gFollowingMonPic_0963_WishiwashiTiles ((u8*)0x08B76400)
#define gFollowingMonPic_0964_MareanieTiles ((u8*)0x08B77000)
#define gFollowingMonPic_0965_ToxapexTiles ((u8*)0x08B77C00)
#define gFollowingMonPic_0966_MudbrayTiles ((u8*)0x08B78800)
#define gFollowingMonPic_0967_MudsdaleTiles ((u8*)0x08B79400)
#define gFollowingMonPic_0968_DewpiderTiles ((u8*)0x08B7A000)
#define gFollowingMonPic_0969_AraquanidTiles ((u8*)0x08B7AC00)
#define gFollowingMonPic_0970_FomantisTiles ((u8*)0x08B7B800)
#define gFollowingMonPic_0971_LurantisTiles ((u8*)0x08B7C400)
#define gFollowingMonPic_0972_MorelullTiles ((u8*)0x08B7D000)
#define gFollowingMonPic_0973_ShiinoticTiles ((u8*)0x08B7DC00)
#define gFollowingMonPic_0974_SalanditTiles ((u8*)0x08B7E800)
#define gFollowingMonPic_0975_SalazzleTiles ((u8*)0x08B7F400)
#define gFollowingMonPic_0976_StuffulTiles ((u8*)0x08B80000)
#define gFollowingMonPic_0977_BewearTiles ((u8*)0x08B80C00)
#define gFollowingMonPic_0978_BounsweetTiles ((u8*)0x08B81800)
#define gFollowingMonPic_0979_SteeneeTiles ((u8*)0x08B82400)
#define gFollowingMonPic_0980_TsareenaTiles ((u8*)0x08B83000)
#define gFollowingMonPic_0981_ComfeyTiles ((u8*)0x08B83C00)
#define gFollowingMonPic_0982_OranguruTiles ((u8*)0x08B84800)
#define gFollowingMonPic_0983_PassimianTiles ((u8*)0x08B85400)
#define gFollowingMonPic_0984_WimpodTiles ((u8*)0x08B86000)
#define gFollowingMonPic_0985_GolisopodTiles ((u8*)0x08B86C00)
#define gFollowingMonPic_0986_SandygastTiles ((u8*)0x08B87800)
#define gFollowingMonPic_0987_PalossandTiles ((u8*)0x08B88400)
#define gFollowingMonPic_0988_PyukumukuTiles ((u8*)0x08B89000)
#define gFollowingMonPic_0989_Type_NullTiles ((u8*)0x08B89C00)
#define gFollowingMonPic_0990_SilvallyTiles ((u8*)0x08B8A800)
#define gFollowingMonPic_0991_MiniorTiles ((u8*)0x08B8B400)
#define gFollowingMonPic_0992_KomalaTiles ((u8*)0x08B8C000)
#define gFollowingMonPic_0993_TurtonatorTiles ((u8*)0x08B8CC00)
#define gFollowingMonPic_0994_TogedemaruTiles ((u8*)0x08B8D800)
#define gFollowingMonPic_0995_MimikyuTiles ((u8*)0x08B8E400)
#define gFollowingMonPic_0996_BruxishTiles ((u8*)0x08B8F000)
#define gFollowingMonPic_0997_DrampaTiles ((u8*)0x08B8FC00)
#define gFollowingMonPic_0998_DhelmiseTiles ((u8*)0x08B90800)
#define gFollowingMonPic_0999_Jangmo_oTiles ((u8*)0x08B91400)
#define gFollowingMonPic_1000_Hakamo_oTiles ((u8*)0x08B92000)
#define gFollowingMonPic_1001_Kommo_oTiles ((u8*)0x08B92C00)
#define gFollowingMonPic_1002_Tapu_KokoTiles ((u8*)0x08B93800)
#define gFollowingMonPic_1003_Tapu_LeleTiles ((u8*)0x08B94400)
#define gFollowingMonPic_1004_Tapu_BuluTiles ((u8*)0x08B95000)
#define gFollowingMonPic_1005_Tapu_FiniTiles ((u8*)0x08B95C00)
#define gFollowingMonPic_1006_CosmogTiles ((u8*)0x08B96800)
#define gFollowingMonPic_1007_CosmoemTiles ((u8*)0x08B97400)
#define gFollowingMonPic_1008_SolgaleoTiles ((u8*)0x08B98000)
#define gFollowingMonPic_1009_LunalaTiles ((u8*)0x08B98C00)
#define gFollowingMonPic_1010_NihilegoTiles ((u8*)0x08B99800)
#define gFollowingMonPic_1011_BuzzwoleTiles ((u8*)0x08B9A400)
#define gFollowingMonPic_1012_PheromosaTiles ((u8*)0x08B9B000)
#define gFollowingMonPic_1013_XurkitreeTiles ((u8*)0x08B9BC00)
#define gFollowingMonPic_1014_CelesteelaTiles ((u8*)0x08B9C800)
#define gFollowingMonPic_1015_KartanaTiles ((u8*)0x08B9D400)
#define gFollowingMonPic_1016_GuzzlordTiles ((u8*)0x08B9E000)
#define gFollowingMonPic_1017_NecrozmaTiles ((u8*)0x08B9EC00)
#define gFollowingMonPic_1018_MagearnaTiles ((u8*)0x08B9F800)
#define gFollowingMonPic_1019_MarshadowTiles ((u8*)0x08BA0400)
#define gFollowingMonPic_1020_Rattata_ATiles ((u8*)0x08BA1000)
#define gFollowingMonPic_1021_Raticate_ATiles ((u8*)0x08BA1C00)
#define gFollowingMonPic_1022_Raichu_ATiles ((u8*)0x08BA2800)
#define gFollowingMonPic_1023_Sandshrew_ATiles ((u8*)0x08BA3400)
#define gFollowingMonPic_1024_Sandslash_ATiles ((u8*)0x08BA4000)
#define gFollowingMonPic_1025_Vulpix_ATiles ((u8*)0x08BA4C00)
#define gFollowingMonPic_1026_Ninetales_ATiles ((u8*)0x08BA5800)
#define gFollowingMonPic_1027_Diglett_ATiles ((u8*)0x08BA6400)
#define gFollowingMonPic_1028_Dugtrio_ATiles ((u8*)0x08BA7000)
#define gFollowingMonPic_1029_Meowth_ATiles ((u8*)0x08BA7C00)
#define gFollowingMonPic_1030_Persian_ATiles ((u8*)0x08BA8800)
#define gFollowingMonPic_1031_Geodude_ATiles ((u8*)0x08BA9400)
#define gFollowingMonPic_1032_Graveler_ATiles ((u8*)0x08BAA000)
#define gFollowingMonPic_1033_Golem_ATiles ((u8*)0x08BAAC00)
#define gFollowingMonPic_1034_Grimer_ATiles ((u8*)0x08BAB800)
#define gFollowingMonPic_1035_Muk_ATiles ((u8*)0x08BAC400)
#define gFollowingMonPic_1037_Exeggutor_ATiles ((u8*)0x08BAD000)
#define gFollowingMonPic_1039_Marowak_ATiles ((u8*)0x08BB0000)
#define gFollowingMonPic_1046_Lycanroc_NTiles ((u8*)0x08BB0C00)
#define gFollowingMonPic_1074_PoipoleTiles ((u8*)0x08BB1800)
#define gFollowingMonPic_1075_NaganadelTiles ((u8*)0x08BB2400)
#define gFollowingMonPic_1076_StakatakaTiles ((u8*)0x08BB3000)
#define gFollowingMonPic_1077_BlacephalonTiles ((u8*)0x08BB3C00)
#define gFollowingMonPic_1078_ZeraoraTiles ((u8*)0x08BB4800)
#define gFollowingMonPic_1079_Necrozma_DMTiles ((u8*)0x08BB5400)
#define gFollowingMonPic_1080_Necrozma_DWTiles ((u8*)0x08BB6000)
#define gFollowingMonPic_1082_Lycanroc_DTiles ((u8*)0x08BB6C00)
#define gFollowingMonPic_1083_MeltanTiles ((u8*)0x08BB7800)
#define gFollowingMonPic_1084_MelmetalTiles ((u8*)0x08BB8400)
#define gFollowingMonPic_1100_Pichu_STiles ((u8*)0x08BB9000)
#define gFollowingMonPic_1102_GrookeyTiles ((u8*)0x08BB9C00)
#define gFollowingMonPic_1103_ThwackeyTiles ((u8*)0x08BBA800)
#define gFollowingMonPic_1104_RillaboomTiles ((u8*)0x08BBB400)
#define gFollowingMonPic_1105_ScorbunnyTiles ((u8*)0x08BBC000)
#define gFollowingMonPic_1106_RabootTiles ((u8*)0x08BBCC00)
#define gFollowingMonPic_1107_CinderaceTiles ((u8*)0x08BBD800)
#define gFollowingMonPic_1108_SobbleTiles ((u8*)0x08BBE400)
#define gFollowingMonPic_1109_DrizzileTiles ((u8*)0x08BBF000)
#define gFollowingMonPic_1110_InteleonTiles ((u8*)0x08BBFC00)
#define gFollowingMonPic_1111_SkwovetTiles ((u8*)0x08BC0800)
#define gFollowingMonPic_1112_GreedentTiles ((u8*)0x08BC1400)
#define gFollowingMonPic_1113_RookideeTiles ((u8*)0x08BC2000)
#define gFollowingMonPic_1114_CorvisquireTiles ((u8*)0x08BC2C00)
#define gFollowingMonPic_1115_CorviknightTiles ((u8*)0x08BC3800)
#define gFollowingMonPic_1116_BlipbugTiles ((u8*)0x08BC4400)
#define gFollowingMonPic_1117_DottlerTiles ((u8*)0x08BC5000)
#define gFollowingMonPic_1118_OrbeetleTiles ((u8*)0x08BC5C00)
#define gFollowingMonPic_1119_NickitTiles ((u8*)0x08BC6800)
#define gFollowingMonPic_1120_ThievulTiles ((u8*)0x08BC7400)
#define gFollowingMonPic_1121_GossifleurTiles ((u8*)0x08BC8000)
#define gFollowingMonPic_1122_EldegossTiles ((u8*)0x08BC8C00)
#define gFollowingMonPic_1123_WoolooTiles ((u8*)0x08BC9800)
#define gFollowingMonPic_1124_DubwoolTiles ((u8*)0x08BCA400)
#define gFollowingMonPic_1125_ChewtleTiles ((u8*)0x08BCB000)
#define gFollowingMonPic_1126_DrednawTiles ((u8*)0x08BCBC00)
#define gFollowingMonPic_1127_YamperTiles ((u8*)0x08BCC800)
#define gFollowingMonPic_1128_BoltundTiles ((u8*)0x08BCD400)
#define gFollowingMonPic_1129_RolycolyTiles ((u8*)0x08BCE000)
#define gFollowingMonPic_1130_CarkolTiles ((u8*)0x08BCEC00)
#define gFollowingMonPic_1131_CoalossalTiles ((u8*)0x08BCF800)
#define gFollowingMonPic_1132_ApplinTiles ((u8*)0x08BD0400)
#define gFollowingMonPic_1133_FlappleTiles ((u8*)0x08BD1000)
#define gFollowingMonPic_1134_AppletunTiles ((u8*)0x08BD1C00)
#define gFollowingMonPic_1135_SilicobraTiles ((u8*)0x08BD2800)
#define gFollowingMonPic_1136_SandacondaTiles ((u8*)0x08BD3400)
#define gFollowingMonPic_1137_CramorantTiles ((u8*)0x08BD4000)
#define gFollowingMonPic_1138_ArrokudaTiles ((u8*)0x08BD4C00)
#define gFollowingMonPic_1139_BarraskewdaTiles ((u8*)0x08BD5800)
#define gFollowingMonPic_1140_ToxelTiles ((u8*)0x08BD6400)
#define gFollowingMonPic_1141_ToxtricityTiles ((u8*)0x08BD7000)
#define gFollowingMonPic_1142_SizzlipedeTiles ((u8*)0x08BD7C00)
#define gFollowingMonPic_1143_CentiskorchTiles ((u8*)0x08BD8800)
#define gFollowingMonPic_1144_ClobbopusTiles ((u8*)0x08BD9400)
#define gFollowingMonPic_1145_GrapploctTiles ((u8*)0x08BDA000)
#define gFollowingMonPic_1146_SinisteaTiles ((u8*)0x08BDAC00)
#define gFollowingMonPic_1147_PolteageistTiles ((u8*)0x08BDB800)
#define gFollowingMonPic_1148_HatennaTiles ((u8*)0x08BDC400)
#define gFollowingMonPic_1149_HattremTiles ((u8*)0x08BDD000)
#define gFollowingMonPic_1150_HattereneTiles ((u8*)0x08BDDC00)
#define gFollowingMonPic_1151_ImpidimpTiles ((u8*)0x08BDE800)
#define gFollowingMonPic_1152_MorgremTiles ((u8*)0x08BDF400)
#define gFollowingMonPic_1153_GrimmsnarlTiles ((u8*)0x08BE0000)
#define gFollowingMonPic_1154_ObstagoonTiles ((u8*)0x08BE0C00)
#define gFollowingMonPic_1155_PerrserkerTiles ((u8*)0x08BE1800)
#define gFollowingMonPic_1156_CursolaTiles ((u8*)0x08BE2400)
#define gFollowingMonPic_1157_SirfetchdTiles ((u8*)0x08BE3000)
#define gFollowingMonPic_1158_Mr_RimeTiles ((u8*)0x08BE3C00)
#define gFollowingMonPic_1159_RunerigusTiles ((u8*)0x08BE4800)
#define gFollowingMonPic_1160_MilceryTiles ((u8*)0x08BE5400)
#define gFollowingMonPic_1161_AlcremieTiles ((u8*)0x08BE6000)
#define gFollowingMonPic_1162_FalinksTiles ((u8*)0x08BE6C00)
#define gFollowingMonPic_1163_PincurchinTiles ((u8*)0x08BE7800)
#define gFollowingMonPic_1164_SnomTiles ((u8*)0x08BE8400)
#define gFollowingMonPic_1165_FrosmothTiles ((u8*)0x08BE9000)
#define gFollowingMonPic_1166_StonjournerTiles ((u8*)0x08BE9C00)
#define gFollowingMonPic_1167_EiscueTiles ((u8*)0x08BEA800)
#define gFollowingMonPic_1168_IndeedeeTiles ((u8*)0x08BEB400)
#define gFollowingMonPic_1169_MorpekoTiles ((u8*)0x08BEC000)
#define gFollowingMonPic_1170_CufantTiles ((u8*)0x08BECC00)
#define gFollowingMonPic_1171_CopperajahTiles ((u8*)0x08BED800)
#define gFollowingMonPic_1172_DracozoltTiles ((u8*)0x08BEE400)
#define gFollowingMonPic_1173_ArctozoltTiles ((u8*)0x08BEF000)
#define gFollowingMonPic_1174_DracovishTiles ((u8*)0x08BEFC00)
#define gFollowingMonPic_1175_ArctovishTiles ((u8*)0x08BF0800)
#define gFollowingMonPic_1176_DuraludonTiles ((u8*)0x08BF1400)
#define gFollowingMonPic_1177_DreepyTiles ((u8*)0x08BF2000)
#define gFollowingMonPic_1178_DrakloakTiles ((u8*)0x08BF2C00)
#define gFollowingMonPic_1179_DragapultTiles ((u8*)0x08BF3800)
#define gFollowingMonPic_1180_ZacianTiles ((u8*)0x08BF4400)
#define gFollowingMonPic_1181_ZamazentaTiles ((u8*)0x08BF5000)
#define gFollowingMonPic_1182_EternatusTiles ((u8*)0x08BF5C00)
#define gFollowingMonPic_1183_KubfuTiles ((u8*)0x08BF6800)
#define gFollowingMonPic_1184_UrshifuTiles ((u8*)0x08BF7400)
#define gFollowingMonPic_1185_ZarudeTiles ((u8*)0x08BF8000)
#define gFollowingMonPic_1186_RegielekiTiles ((u8*)0x08BF8C00)
#define gFollowingMonPic_1187_RegidragoTiles ((u8*)0x08BF9800)
#define gFollowingMonPic_1188_GlastrierTiles ((u8*)0x08BFA400)
#define gFollowingMonPic_1189_SpectrierTiles ((u8*)0x08BFB000)
#define gFollowingMonPic_1190_CalyrexTiles ((u8*)0x08BFBC00)
#define gFollowingMonPic_1193_Toxtricity_LKTiles ((u8*)0x08BFC800)
#define gFollowingMonPic_1203_Indeedee_FTiles ((u8*)0x08BFD400)
#define gFollowingMonPic_1205_Zacian_CTiles ((u8*)0x08BFE000)
#define gFollowingMonPic_1206_Zamazenta_CTiles ((u8*)0x08BFEC00)
#define gFollowingMonPic_1210_Calyrex_IRTiles ((u8*)0x08BFF800)
#define gFollowingMonPic_1211_Calyrex_SRTiles ((u8*)0x08C00400)
#define gFollowingMonPic_1212_Meowth_GTiles ((u8*)0x08C01000)
#define gFollowingMonPic_1213_Ponyta_GTiles ((u8*)0x08C01C00)
#define gFollowingMonPic_1214_Rapidash_GTiles ((u8*)0x08C02800)
#define gFollowingMonPic_1215_Slowpoke_GTiles ((u8*)0x08C03400)
#define gFollowingMonPic_1216_Slowbro_GTiles ((u8*)0x08C04000)
#define gFollowingMonPic_1217_Farfetchd_GTiles ((u8*)0x08C05000)
#define gFollowingMonPic_1219_Weezing_GTiles ((u8*)0x08C05C00)
#define gFollowingMonPic_1220_Mr_Mime_GTiles ((u8*)0x08C06800)
#define gFollowingMonPic_1221_Articuno_GTiles ((u8*)0x08C07400)
#define gFollowingMonPic_1222_Zapdos_GTiles ((u8*)0x08C08000)
#define gFollowingMonPic_1223_Moltres_GTiles ((u8*)0x08C08C00)
#define gFollowingMonPic_1224_Slowking_GTiles ((u8*)0x08C09800)
#define gFollowingMonPic_1225_Corsola_GTiles ((u8*)0x08C0A400)
#define gFollowingMonPic_1226_Zigzagoon_GTiles ((u8*)0x08C0B000)
#define gFollowingMonPic_1227_Linoone_GTiles ((u8*)0x08C0BC00)
#define gFollowingMonPic_1229_Darumaka_GTiles ((u8*)0x08C0C800)
#define gFollowingMonPic_1230_Darmanitan_GTiles ((u8*)0x08C0D400)
#define gFollowingMonPic_1232_Yamask_GTiles ((u8*)0x08C0E000)
#define gFollowingMonPic_1233_Stunfisk_GTiles ((u8*)0x08C0EC00)
#define gFollowingMonPic_1234_Growlithe_HTiles ((u8*)0x08C0F800)
#define gFollowingMonPic_1235_Arcanine_HTiles ((u8*)0x08C10400)
#define gFollowingMonPic_1236_Voltorb_HTiles ((u8*)0x08C11000)
#define gFollowingMonPic_1237_Electrode_HTiles ((u8*)0x08C11C00)
#define gFollowingMonPic_1238_Typhlosion_HTiles ((u8*)0x08C12800)
#define gFollowingMonPic_1239_Qwilfish_HTiles ((u8*)0x08C13400)
#define gFollowingMonPic_1240_Sneasel_HTiles ((u8*)0x08C14000)
#define gFollowingMonPic_1242_Lilligant_HTiles ((u8*)0x08C14C00)
#define gFollowingMonPic_1243_Basculin_WTiles ((u8*)0x08C15800)
#define gFollowingMonPic_1244_Zorua_HTiles ((u8*)0x08C16400)
#define gFollowingMonPic_1245_Zoroark_HTiles ((u8*)0x08C17000)
#define gFollowingMonPic_1246_Braviary_HTiles ((u8*)0x08C17C00)
#define gFollowingMonPic_1247_Sliggoo_HTiles ((u8*)0x08C18800)
#define gFollowingMonPic_1248_Goodra_HTiles ((u8*)0x08C19400)
#define gFollowingMonPic_1249_Avalugg_HTiles ((u8*)0x08C1A000)
#define gFollowingMonPic_1250_Decidueye_HTiles ((u8*)0x08C1AC00)
#define gFollowingMonPic_1251_WyrdeerTiles ((u8*)0x08C1B800)
#define gFollowingMonPic_1252_KleavorTiles ((u8*)0x08C1C400)
#define gFollowingMonPic_1253_UrsalunaTiles ((u8*)0x08C1D000)
#define gFollowingMonPic_1254_BasculegionTiles ((u8*)0x08C1DC00)
#define gFollowingMonPic_1255_Basculegion_FTiles ((u8*)0x08C1E800)
#define gFollowingMonPic_1256_SneaslerTiles ((u8*)0x08C1F400)
#define gFollowingMonPic_1257_OverqwilTiles ((u8*)0x08C20000)
#define gFollowingMonPic_1258_EnamorusTiles ((u8*)0x08C20C00)
#define gFollowingMonPic_1259_Enamorus_TTiles ((u8*)0x08C23C00)
#define gFollowingMonPic_1294_SprigatitoTiles ((u8*)0x08C26C00)
#define gFollowingMonPic_1295_FloragatoTiles ((u8*)0x08C27800)
#define gFollowingMonPic_1296_MeowscaradaTiles ((u8*)0x08C28400)
#define gFollowingMonPic_1297_FuecocoTiles ((u8*)0x08C29000)
#define gFollowingMonPic_1298_CrocalorTiles ((u8*)0x08C29C00)
#define gFollowingMonPic_1299_SkeledirgeTiles ((u8*)0x08C2A800)
#define gFollowingMonPic_1300_QuaxlyTiles ((u8*)0x08C2B400)
#define gFollowingMonPic_1301_QuaxwellTiles ((u8*)0x08C2C000)
#define gFollowingMonPic_1302_QuaquavalTiles ((u8*)0x08C2CC00)
#define gFollowingMonPic_1303_LechonkTiles ((u8*)0x08C2D800)
#define gFollowingMonPic_1304_OinkologneTiles ((u8*)0x08C2E400)
#define gFollowingMonPic_1305_TarountulaTiles ((u8*)0x08C2F000)
#define gFollowingMonPic_1306_SpidopsTiles ((u8*)0x08C2FC00)
#define gFollowingMonPic_1307_NymbleTiles ((u8*)0x08C30800)
#define gFollowingMonPic_1308_LokixTiles ((u8*)0x08C31400)
#define gFollowingMonPic_1309_PawmiTiles ((u8*)0x08C32000)
#define gFollowingMonPic_1310_PawmoTiles ((u8*)0x08C32C00)
#define gFollowingMonPic_1311_PawmotTiles ((u8*)0x08C33800)
#define gFollowingMonPic_1312_TandemausTiles ((u8*)0x08C34400)
#define gFollowingMonPic_1313_MausholdTiles ((u8*)0x08C35000)
#define gFollowingMonPic_1314_FidoughTiles ((u8*)0x08C35C00)
#define gFollowingMonPic_1315_DachsbunTiles ((u8*)0x08C36800)
#define gFollowingMonPic_1316_SmolivTiles ((u8*)0x08C37400)
#define gFollowingMonPic_1317_DollivTiles ((u8*)0x08C38000)
#define gFollowingMonPic_1318_ArbolivaTiles ((u8*)0x08C38C00)
#define gFollowingMonPic_1319_SquawkabillyTiles ((u8*)0x08C39800)
#define gFollowingMonPic_1320_NacliTiles ((u8*)0x08C3A400)
#define gFollowingMonPic_1321_NaclstackTiles ((u8*)0x08C3B000)
#define gFollowingMonPic_1322_GarganaclTiles ((u8*)0x08C3BC00)
#define gFollowingMonPic_1323_CharcadetTiles ((u8*)0x08C3C800)
#define gFollowingMonPic_1324_ArmarougeTiles ((u8*)0x08C3D400)
#define gFollowingMonPic_1325_CeruledgeTiles ((u8*)0x08C3E000)
#define gFollowingMonPic_1326_TadbulbTiles ((u8*)0x08C3EC00)
#define gFollowingMonPic_1327_BelliboltTiles ((u8*)0x08C3F800)
#define gFollowingMonPic_1328_WattrelTiles ((u8*)0x08C40400)
#define gFollowingMonPic_1329_KilowattrelTiles ((u8*)0x08C41000)
#define gFollowingMonPic_1330_MaschiffTiles ((u8*)0x08C41C00)
#define gFollowingMonPic_1331_MabosstiffTiles ((u8*)0x08C42800)
#define gFollowingMonPic_1332_ShroodleTiles ((u8*)0x08C43400)
#define gFollowingMonPic_1333_GrafaiaiTiles ((u8*)0x08C44000)
#define gFollowingMonPic_1334_BramblinTiles ((u8*)0x08C44C00)
#define gFollowingMonPic_1335_BrambleghastTiles ((u8*)0x08C45800)
#define gFollowingMonPic_1336_ToedscoolTiles ((u8*)0x08C46400)
#define gFollowingMonPic_1337_ToedscruelTiles ((u8*)0x08C47000)
#define gFollowingMonPic_1338_KlawfTiles ((u8*)0x08C47C00)
#define gFollowingMonPic_1339_CapsakidTiles ((u8*)0x08C48800)
#define gFollowingMonPic_1340_ScovillainTiles ((u8*)0x08C49400)
#define gFollowingMonPic_1341_RellorTiles ((u8*)0x08C4A400)
#define gFollowingMonPic_1342_RabscaTiles ((u8*)0x08C4B000)
#define gFollowingMonPic_1343_FlittleTiles ((u8*)0x08C4BC00)
#define gFollowingMonPic_1344_EspathraTiles ((u8*)0x08C4C800)
#define gFollowingMonPic_1345_TinkatinkTiles ((u8*)0x08C4D400)
#define gFollowingMonPic_1346_TinkatuffTiles ((u8*)0x08C4E400)
#define gFollowingMonPic_1347_TinkatonTiles ((u8*)0x08C4F400)
#define gFollowingMonPic_1348_WiglettTiles ((u8*)0x08C50400)
#define gFollowingMonPic_1349_WugtrioTiles ((u8*)0x08C51000)
#define gFollowingMonPic_1350_BombirdierTiles ((u8*)0x08C51C00)
#define gFollowingMonPic_1351_FinizenTiles ((u8*)0x08C52800)
#define gFollowingMonPic_1352_PalafinTiles ((u8*)0x08C53400)
#define gFollowingMonPic_1353_VaroomTiles ((u8*)0x08C54000)
#define gFollowingMonPic_1354_RevavroomTiles ((u8*)0x08C54C00)
#define gFollowingMonPic_1355_CyclizarTiles ((u8*)0x08C55800)
#define gFollowingMonPic_1356_OrthwormTiles ((u8*)0x08C56400)
#define gFollowingMonPic_1357_GlimmetTiles ((u8*)0x08C57000)
#define gFollowingMonPic_1358_GlimmoraTiles ((u8*)0x08C57C00)
#define gFollowingMonPic_1359_GreavardTiles ((u8*)0x08C58800)
#define gFollowingMonPic_1360_HoundstoneTiles ((u8*)0x08C59400)
#define gFollowingMonPic_1361_FlamigoTiles ((u8*)0x08C5A000)
#define gFollowingMonPic_1362_CetoddleTiles ((u8*)0x08C5AC00)
#define gFollowingMonPic_1363_CetitanTiles ((u8*)0x08C5B800)
#define gFollowingMonPic_1364_VeluzaTiles ((u8*)0x08C5C400)
#define gFollowingMonPic_1365_DondozoTiles ((u8*)0x08C5D000)
#define gFollowingMonPic_1366_TatsugiriTiles ((u8*)0x08C60000)
#define gFollowingMonPic_1367_AnnihilapeTiles ((u8*)0x08C60C00)
#define gFollowingMonPic_1368_ClodsireTiles ((u8*)0x08C61800)
#define gFollowingMonPic_1369_FarigirafTiles ((u8*)0x08C62400)
#define gFollowingMonPic_1370_DudunsparceTiles ((u8*)0x08C63000)
#define gFollowingMonPic_1371_KingambitTiles ((u8*)0x08C63C00)
#define gFollowingMonPic_1372_Great_TuskTiles ((u8*)0x08C66C00)
#define gFollowingMonPic_1373_Scream_TailTiles ((u8*)0x08C67800)
#define gFollowingMonPic_1374_Brute_BonnetTiles ((u8*)0x08C68400)
#define gFollowingMonPic_1375_Flutter_ManeTiles ((u8*)0x08C69000)
#define gFollowingMonPic_1376_Slither_WingTiles ((u8*)0x08C69C00)
#define gFollowingMonPic_1377_Sandy_ShocksTiles ((u8*)0x08C6A800)
#define gFollowingMonPic_1378_Iron_TreadsTiles ((u8*)0x08C6B400)
#define gFollowingMonPic_1379_Iron_BundleTiles ((u8*)0x08C6C000)
#define gFollowingMonPic_1380_Iron_HandsTiles ((u8*)0x08C6CC00)
#define gFollowingMonPic_1381_Iron_JugulisTiles ((u8*)0x08C6D800)
#define gFollowingMonPic_1382_Iron_MothTiles ((u8*)0x08C6E400)
#define gFollowingMonPic_1383_Iron_ThornsTiles ((u8*)0x08C6F000)
#define gFollowingMonPic_1384_FrigibaxTiles ((u8*)0x08C6FC00)
#define gFollowingMonPic_1385_ArctibaxTiles ((u8*)0x08C70800)
#define gFollowingMonPic_1386_BaxcaliburTiles ((u8*)0x08C71400)
#define gFollowingMonPic_1387_GimmighoulTiles ((u8*)0x08C72000)
#define gFollowingMonPic_1388_GholdengoTiles ((u8*)0x08C72C00)
#define gFollowingMonPic_1389_Wo_ChienTiles ((u8*)0x08C73800)
#define gFollowingMonPic_1390_Chien_PaoTiles ((u8*)0x08C74400)
#define gFollowingMonPic_1391_Ting_LuTiles ((u8*)0x08C75000)
#define gFollowingMonPic_1392_Chi_YuTiles ((u8*)0x08C75C00)
#define gFollowingMonPic_1393_Roaring_MoonTiles ((u8*)0x08C76800)
#define gFollowingMonPic_1394_Iron_ValiantTiles ((u8*)0x08C77400)
#define gFollowingMonPic_1395_KoraidonTiles ((u8*)0x08C78000)
#define gFollowingMonPic_1396_MiraidonTiles ((u8*)0x08C7B000)
#define gFollowingMonPic_1397_Walking_WakeTiles ((u8*)0x08C7E000)
#define gFollowingMonPic_1398_Iron_LeavesTiles ((u8*)0x08C7EC00)
#define gFollowingMonPic_1399_Oinkologne_FTiles ((u8*)0x08C7F800)
#define gFollowingMonPic_1406_Dudunsparce_2Tiles ((u8*)0x08C80400)
#define gFollowingMonPic_1407_Tauros_CTiles ((u8*)0x08C83400)
#define gFollowingMonPic_1408_Tauros_BTiles ((u8*)0x08C84000)
#define gFollowingMonPic_1409_Tauros_ATiles ((u8*)0x08C84C00)
#define gFollowingMonPic_1410_Wooper_PTiles ((u8*)0x08C85800)
#define gFollowingMonPic_1420_DipplinTiles ((u8*)0x08C86400)
#define gFollowingMonPic_1421_PoltchageistTiles ((u8*)0x08C87000)
#define gFollowingMonPic_1422_SinistchaTiles ((u8*)0x08C87C00)
#define gFollowingMonPic_1423_OkidogiTiles ((u8*)0x08C88800)
#define gFollowingMonPic_1424_MunkidoriTiles ((u8*)0x08C89400)
#define gFollowingMonPic_1425_FezandipitiTiles ((u8*)0x08C8A000)
#define gFollowingMonPic_1426_OgerponTiles ((u8*)0x08C8AC00)
#define gFollowingMonPic_1426_Ogerpon_1Tiles ((u8*)0x08C8B800)
#define gFollowingMonPic_1426_Ogerpon_2Tiles ((u8*)0x08C8C400)
#define gFollowingMonPic_1426_Ogerpon_3Tiles ((u8*)0x08C8D000)
#define gFollowingMonPic_1430_ArchaludonTiles ((u8*)0x08C8DC00)
#define gFollowingMonPic_1431_HydrappleTiles ((u8*)0x08C8E800)
#define gFollowingMonPic_1432_Gouging_FireTiles ((u8*)0x08C91800)
#define gFollowingMonPic_1433_Raging_BoltTiles ((u8*)0x08C92400)
#define gFollowingMonPic_1434_Iron_BoulderTiles ((u8*)0x08C95400)
#define gFollowingMonPic_1435_Iron_CrownTiles ((u8*)0x08C96000)
#define gFollowingMonPic_1436_TerapagosTiles ((u8*)0x08C96C00)
#define gFollowingMonPic_1436_Terapagos_1Tiles ((u8*)0x08C97800)
#define gFollowingMonPic_1438_PecharuntTiles ((u8*)0x08C98400)
#define gFollowingMonPic_1440_Maushold_FTiles ((u8*)0x08C99000)
#define gFollowingMonPic_1493_Ursaluna_BMTiles ((u8*)0x08C99C00)

// Palette declarations
//Normal Pal
extern const u16 gFollowingMonPic_AlcremieStrawberryNormalPal[];
extern const u16 gFollowingMonPic_ArcanineHisuianNormalPal[];
extern const u16 gFollowingMonPic_ArceusBugNormalPal[];
extern const u16 gFollowingMonPic_ArceusDarkNormalPal[];
extern const u16 gFollowingMonPic_ArceusDragonNormalPal[];
extern const u16 gFollowingMonPic_ArceusElectricNormalPal[];
extern const u16 gFollowingMonPic_ArceusFairyNormalPal[];
extern const u16 gFollowingMonPic_ArceusFightingNormalPal[];
extern const u16 gFollowingMonPic_ArceusFireNormalPal[];
extern const u16 gFollowingMonPic_ArceusFlyingNormalPal[];
extern const u16 gFollowingMonPic_ArceusGhostNormalPal[];
extern const u16 gFollowingMonPic_ArceusGrassNormalPal[];
extern const u16 gFollowingMonPic_ArceusGroundNormalPal[];
extern const u16 gFollowingMonPic_ArceusIceNormalPal[];
extern const u16 gFollowingMonPic_ArceusPoisonNormalPal[];
extern const u16 gFollowingMonPic_ArceusPsychicNormalPal[];
extern const u16 gFollowingMonPic_ArceusRockNormalPal[];
extern const u16 gFollowingMonPic_ArceusSteelNormalPal[];
extern const u16 gFollowingMonPic_ArceusWaterNormalPal[];
extern const u16 gFollowingMonPic_ArticunoGalarianNormalPal[];
extern const u16 gFollowingMonPic_AvaluggHisuianNormalPal[];
extern const u16 gFollowingMonPic_BasculegionFemaleNormalPal[];
extern const u16 gFollowingMonPic_BasculinBlue_stripedNormalPal[];
extern const u16 gFollowingMonPic_BasculinHisuianNormalPal[];
extern const u16 gFollowingMonPic_BasculinWhite_stripedNormalPal[];
extern const u16 gFollowingMonPic_BraviaryHisuianNormalPal[];
extern const u16 gFollowingMonPic_BurmySandy_cloakNormalPal[];
extern const u16 gFollowingMonPic_BurmyTrash_cloakNormalPal[];
extern const u16 gFollowingMonPic_CalyrexIce_riderNormalPal[];
extern const u16 gFollowingMonPic_CalyrexShadow_riderNormalPal[];
extern const u16 gFollowingMonPic_CastformRainyNormalPal[];
extern const u16 gFollowingMonPic_CastformSnowyNormalPal[];
extern const u16 gFollowingMonPic_CastformSunnyNormalPal[];
extern const u16 gFollowingMonPic_CorsolaGalarianNormalPal[];
extern const u16 gFollowingMonPic_DarmanitanGalarianNormalPal[];
extern const u16 gFollowingMonPic_DarumakaGalarianNormalPal[];
extern const u16 gFollowingMonPic_DecidueyeHisuianNormalPal[];
extern const u16 gFollowingMonPic_DeerlingAutumnNormalPal[];
extern const u16 gFollowingMonPic_DeerlingSummerNormalPal[];
extern const u16 gFollowingMonPic_DeerlingWinterNormalPal[];
extern const u16 gFollowingMonPic_DeoxysAttackNormalPal[];
extern const u16 gFollowingMonPic_DeoxysDefenseNormalPal[];
extern const u16 gFollowingMonPic_DeoxysSpeedNormalPal[];
extern const u16 gFollowingMonPic_DialgaOriginNormalPal[];
extern const u16 gFollowingMonPic_DiglettAlolanNormalPal[];
extern const u16 gFollowingMonPic_DudunsparceThree_segmentNormalPal[];
extern const u16 gFollowingMonPic_DugtrioAlolanNormalPal[];
extern const u16 gFollowingMonPic_ElectrodeHisuianNormalPal[];
extern const u16 gFollowingMonPic_EnamorusTherianNormalPal[];
extern const u16 gFollowingMonPic_ExeggutorAlolanNormalPal[];
extern const u16 gFollowingMonPic_FarfetchdGalarianNormalPal[];
extern const u16 gFollowingMonPic_FlabebeBlue_flowerNormalPal[];
extern const u16 gFollowingMonPic_FlabebeOrange_flowerNormalPal[];
extern const u16 gFollowingMonPic_FlabebeWhite_flowerNormalPal[];
extern const u16 gFollowingMonPic_FlabebeYellow_flowerNormalPal[];
extern const u16 gFollowingMonPic_FloetteBlue_flowerNormalPal[];
extern const u16 gFollowingMonPic_FloetteEternal_flowerNormalPal[];
extern const u16 gFollowingMonPic_FloetteOrange_flowerNormalPal[];
extern const u16 gFollowingMonPic_FloetteWhite_flowerNormalPal[];
extern const u16 gFollowingMonPic_FloetteYellow_flowerNormalPal[];
extern const u16 gFollowingMonPic_FlorgesBlue_flowerNormalPal[];
extern const u16 gFollowingMonPic_FlorgesOrange_flowerNormalPal[];
extern const u16 gFollowingMonPic_FlorgesWhite_flowerNormalPal[];
extern const u16 gFollowingMonPic_FlorgesYellow_flowerNormalPal[];
extern const u16 gFollowingMonPic_Frillish_FNormalPal[];
extern const u16 gFollowingMonPic_FurfrouDandy_trimNormalPal[];
extern const u16 gFollowingMonPic_FurfrouDebutante_trimNormalPal[];
extern const u16 gFollowingMonPic_FurfrouDiamond_trimNormalPal[];
extern const u16 gFollowingMonPic_FurfrouHeart_trimNormalPal[];
extern const u16 gFollowingMonPic_FurfrouKabuki_trimNormalPal[];
extern const u16 gFollowingMonPic_FurfrouLa_reine_trimNormalPal[];
extern const u16 gFollowingMonPic_FurfrouMatron_trimNormalPal[];
extern const u16 gFollowingMonPic_FurfrouPharaoh_trimNormalPal[];
extern const u16 gFollowingMonPic_FurfrouStar_trimNormalPal[];
extern const u16 gFollowingMonPic_GastrodonEast_seaNormalPal[];
extern const u16 gFollowingMonPic_GeodudeAlolanNormalPal[];
extern const u16 gFollowingMonPic_GiratinaOriginNormalPal[];
extern const u16 gFollowingMonPic_GolemAlolanNormalPal[];
extern const u16 gFollowingMonPic_GoodraHisuianNormalPal[];
extern const u16 gFollowingMonPic_GravelerAlolanNormalPal[];
extern const u16 gFollowingMonPic_GrimerAlolanNormalPal[];
extern const u16 gFollowingMonPic_GrowlitheHisuianNormalPal[];
extern const u16 gFollowingMonPic_HoopaUnboundNormalPal[];
extern const u16 gFollowingMonPic_IndeedeeFemaleNormalPal[];
extern const u16 gFollowingMonPic_Jellicent_FNormalPal[];
extern const u16 gFollowingMonPic_KyuremBlackNormalPal[];
extern const u16 gFollowingMonPic_KyuremWhiteNormalPal[];
extern const u16 gFollowingMonPic_LandorusTherianNormalPal[];
extern const u16 gFollowingMonPic_LilligantHisuianNormalPal[];
extern const u16 gFollowingMonPic_LinooneGalarianNormalPal[];
extern const u16 gFollowingMonPic_LycanrocDuskNormalPal[];
extern const u16 gFollowingMonPic_LycanrocMidnightNormalPal[];
extern const u16 gFollowingMonPic_MagearnaOriginal_colorNormalPal[];
extern const u16 gFollowingMonPic_MarowakAlolanNormalPal[];
extern const u16 gFollowingMonPic_MausholdFourNormalPal[];
extern const u16 gFollowingMonPic_MeloettaPirouetteNormalPal[];
extern const u16 gFollowingMonPic_MeowsticFemaleNormalPal[];
extern const u16 gFollowingMonPic_MeowthAlolanNormalPal[];
extern const u16 gFollowingMonPic_MeowthGalarianNormalPal[];
extern const u16 gFollowingMonPic_MoltresGalarianNormalPal[];
extern const u16 gFollowingMonPic_Mr_mimeGalarianNormalPal[];
extern const u16 gFollowingMonPic_MukAlolanNormalPal[];
extern const u16 gFollowingMonPic_NecrozmaDawn_wingsNormalPal[];
extern const u16 gFollowingMonPic_NecrozmaDusk_maneNormalPal[];
extern const u16 gFollowingMonPic_NinetalesAlolanNormalPal[];
extern const u16 gFollowingMonPic_OgerponCornerstoneNormalPal[];
extern const u16 gFollowingMonPic_OgerponHearthflameNormalPal[];
extern const u16 gFollowingMonPic_OgerponWellspringNormalPal[];
extern const u16 gFollowingMonPic_OinkologneFemaleNormalPal[];
extern const u16 gFollowingMonPic_OricorioPauNormalPal[];
extern const u16 gFollowingMonPic_OricorioPom_pomNormalPal[];
extern const u16 gFollowingMonPic_OricorioSensuNormalPal[];
extern const u16 gFollowingMonPic_PalafinHeroNormalPal[];
extern const u16 gFollowingMonPic_PalkiaOriginNormalPal[];
extern const u16 gFollowingMonPic_PersianAlolanNormalPal[];
extern const u16 gFollowingMonPic_PichuSpiky_earedNormalPal[];
extern const u16 gFollowingMonPic_AbomasnowNormalPal[];
extern const u16 gFollowingMonPic_AbraNormalPal[];
extern const u16 gFollowingMonPic_AbsolNormalPal[];
extern const u16 gFollowingMonPic_AccelgorNormalPal[];
extern const u16 gFollowingMonPic_AegislashNormalPal[];
extern const u16 gFollowingMonPic_AerodactylNormalPal[];
extern const u16 gFollowingMonPic_AggronNormalPal[];
extern const u16 gFollowingMonPic_AipomNormalPal[];
extern const u16 gFollowingMonPic_AlakazamNormalPal[];
extern const u16 gFollowingMonPic_AlomomolaNormalPal[];
extern const u16 gFollowingMonPic_AltariaNormalPal[];
extern const u16 gFollowingMonPic_AmauraNormalPal[];
extern const u16 gFollowingMonPic_AmbipomNormalPal[];
extern const u16 gFollowingMonPic_AmoongussNormalPal[];
extern const u16 gFollowingMonPic_AmpharosNormalPal[];
extern const u16 gFollowingMonPic_AnnihilapeNormalPal[];
extern const u16 gFollowingMonPic_AnorithNormalPal[];
extern const u16 gFollowingMonPic_AppletunNormalPal[];
extern const u16 gFollowingMonPic_ApplinNormalPal[];
extern const u16 gFollowingMonPic_AraquanidNormalPal[];
extern const u16 gFollowingMonPic_ArbokNormalPal[];
extern const u16 gFollowingMonPic_ArbolivaNormalPal[];
extern const u16 gFollowingMonPic_ArcanineNormalPal[];
extern const u16 gFollowingMonPic_ArceusNormalPal[];
extern const u16 gFollowingMonPic_ArchaludonNormalPal[];
extern const u16 gFollowingMonPic_ArchenNormalPal[];
extern const u16 gFollowingMonPic_ArcheopsNormalPal[];
extern const u16 gFollowingMonPic_ArctibaxNormalPal[];
extern const u16 gFollowingMonPic_ArctovishNormalPal[];
extern const u16 gFollowingMonPic_ArctozoltNormalPal[];
extern const u16 gFollowingMonPic_AriadosNormalPal[];
extern const u16 gFollowingMonPic_ArmaldoNormalPal[];
extern const u16 gFollowingMonPic_ArmarougeNormalPal[];
extern const u16 gFollowingMonPic_AromatisseNormalPal[];
extern const u16 gFollowingMonPic_AronNormalPal[];
extern const u16 gFollowingMonPic_ArrokudaNormalPal[];
extern const u16 gFollowingMonPic_ArticunoNormalPal[];
extern const u16 gFollowingMonPic_AudinoNormalPal[];
extern const u16 gFollowingMonPic_AurorusNormalPal[];
extern const u16 gFollowingMonPic_AvaluggNormalPal[];
extern const u16 gFollowingMonPic_AxewNormalPal[];
extern const u16 gFollowingMonPic_AzelfNormalPal[];
extern const u16 gFollowingMonPic_AzumarillNormalPal[];
extern const u16 gFollowingMonPic_AzurillNormalPal[];
extern const u16 gFollowingMonPic_BagonNormalPal[];
extern const u16 gFollowingMonPic_BaltoyNormalPal[];
extern const u16 gFollowingMonPic_BanetteNormalPal[];
extern const u16 gFollowingMonPic_BarbaracleNormalPal[];
extern const u16 gFollowingMonPic_BarboachNormalPal[];
extern const u16 gFollowingMonPic_BarraskewdaNormalPal[];
extern const u16 gFollowingMonPic_BasculegionNormalPal[];
extern const u16 gFollowingMonPic_BasculinNormalPal[];
extern const u16 gFollowingMonPic_BastiodonNormalPal[];
extern const u16 gFollowingMonPic_BaxcaliburNormalPal[];
extern const u16 gFollowingMonPic_BayleefNormalPal[];
extern const u16 gFollowingMonPic_BearticNormalPal[];
extern const u16 gFollowingMonPic_BeautiflyNormalPal[];
extern const u16 gFollowingMonPic_BeedrillNormalPal[];
extern const u16 gFollowingMonPic_BeheeyemNormalPal[];
extern const u16 gFollowingMonPic_BeldumNormalPal[];
extern const u16 gFollowingMonPic_BelliboltNormalPal[];
extern const u16 gFollowingMonPic_BellossomNormalPal[];
extern const u16 gFollowingMonPic_BellsproutNormalPal[];
extern const u16 gFollowingMonPic_BergmiteNormalPal[];
extern const u16 gFollowingMonPic_BewearNormalPal[];
extern const u16 gFollowingMonPic_BibarelNormalPal[];
extern const u16 gFollowingMonPic_BidoofNormalPal[];
extern const u16 gFollowingMonPic_BinacleNormalPal[];
extern const u16 gFollowingMonPic_BisharpNormalPal[];
extern const u16 gFollowingMonPic_BlacephalonNormalPal[];
extern const u16 gFollowingMonPic_BlastoiseNormalPal[];
extern const u16 gFollowingMonPic_BlazikenNormalPal[];
extern const u16 gFollowingMonPic_BlipbugNormalPal[];
extern const u16 gFollowingMonPic_BlisseyNormalPal[];
extern const u16 gFollowingMonPic_BlitzleNormalPal[];
extern const u16 gFollowingMonPic_BoldoreNormalPal[];
extern const u16 gFollowingMonPic_BoltundNormalPal[];
extern const u16 gFollowingMonPic_BombirdierNormalPal[];
extern const u16 gFollowingMonPic_BonslyNormalPal[];
extern const u16 gFollowingMonPic_BouffalantNormalPal[];
extern const u16 gFollowingMonPic_BounsweetNormalPal[];
extern const u16 gFollowingMonPic_BraixenNormalPal[];
extern const u16 gFollowingMonPic_BrambleghastNormalPal[];
extern const u16 gFollowingMonPic_BramblinNormalPal[];
extern const u16 gFollowingMonPic_BraviaryNormalPal[];
extern const u16 gFollowingMonPic_BreloomNormalPal[];
extern const u16 gFollowingMonPic_BrionneNormalPal[];
extern const u16 gFollowingMonPic_BronzongNormalPal[];
extern const u16 gFollowingMonPic_BronzorNormalPal[];
extern const u16 gFollowingMonPic_Brute_bonnetNormalPal[];
extern const u16 gFollowingMonPic_BruxishNormalPal[];
extern const u16 gFollowingMonPic_BudewNormalPal[];
extern const u16 gFollowingMonPic_BuizelNormalPal[];
extern const u16 gFollowingMonPic_BulbasaurNormalPal[];
extern const u16 gFollowingMonPic_BunearyNormalPal[];
extern const u16 gFollowingMonPic_BunnelbyNormalPal[];
extern const u16 gFollowingMonPic_BurmyNormalPal[];
extern const u16 gFollowingMonPic_ButterfreeNormalPal[];
extern const u16 gFollowingMonPic_BuzzwoleNormalPal[];
extern const u16 gFollowingMonPic_CacneaNormalPal[];
extern const u16 gFollowingMonPic_CacturneNormalPal[];
extern const u16 gFollowingMonPic_CalyrexNormalPal[];
extern const u16 gFollowingMonPic_CameruptNormalPal[];
extern const u16 gFollowingMonPic_CapsakidNormalPal[];
extern const u16 gFollowingMonPic_CarbinkNormalPal[];
extern const u16 gFollowingMonPic_CarkolNormalPal[];
extern const u16 gFollowingMonPic_CarnivineNormalPal[];
extern const u16 gFollowingMonPic_CarracostaNormalPal[];
extern const u16 gFollowingMonPic_CarvanhaNormalPal[];
extern const u16 gFollowingMonPic_CascoonNormalPal[];
extern const u16 gFollowingMonPic_CastformNormalPal[];
extern const u16 gFollowingMonPic_CaterpieNormalPal[];
extern const u16 gFollowingMonPic_CelebiNormalPal[];
extern const u16 gFollowingMonPic_CelesteelaNormalPal[];
extern const u16 gFollowingMonPic_CentiskorchNormalPal[];
extern const u16 gFollowingMonPic_CeruledgeNormalPal[];
extern const u16 gFollowingMonPic_CetitanNormalPal[];
extern const u16 gFollowingMonPic_CetoddleNormalPal[];
extern const u16 gFollowingMonPic_ChandelureNormalPal[];
extern const u16 gFollowingMonPic_ChanseyNormalPal[];
extern const u16 gFollowingMonPic_CharcadetNormalPal[];
extern const u16 gFollowingMonPic_CharizardNormalPal[];
extern const u16 gFollowingMonPic_CharjabugNormalPal[];
extern const u16 gFollowingMonPic_CharmanderNormalPal[];
extern const u16 gFollowingMonPic_CharmeleonNormalPal[];
extern const u16 gFollowingMonPic_ChatotNormalPal[];
extern const u16 gFollowingMonPic_CherrimNormalPal[];
extern const u16 gFollowingMonPic_CherubiNormalPal[];
extern const u16 gFollowingMonPic_ChesnaughtNormalPal[];
extern const u16 gFollowingMonPic_ChespinNormalPal[];
extern const u16 gFollowingMonPic_ChewtleNormalPal[];
extern const u16 gFollowingMonPic_Chien_paoNormalPal[];
extern const u16 gFollowingMonPic_ChikoritaNormalPal[];
extern const u16 gFollowingMonPic_ChimcharNormalPal[];
extern const u16 gFollowingMonPic_ChimechoNormalPal[];
extern const u16 gFollowingMonPic_ChinchouNormalPal[];
extern const u16 gFollowingMonPic_ChinglingNormalPal[];
extern const u16 gFollowingMonPic_Chi_yuNormalPal[];
extern const u16 gFollowingMonPic_CinccinoNormalPal[];
extern const u16 gFollowingMonPic_CinderaceNormalPal[];
extern const u16 gFollowingMonPic_ClamperlNormalPal[];
extern const u16 gFollowingMonPic_ClauncherNormalPal[];
extern const u16 gFollowingMonPic_ClawitzerNormalPal[];
extern const u16 gFollowingMonPic_ClaydolNormalPal[];
extern const u16 gFollowingMonPic_ClefableNormalPal[];
extern const u16 gFollowingMonPic_ClefairyNormalPal[];
extern const u16 gFollowingMonPic_CleffaNormalPal[];
extern const u16 gFollowingMonPic_ClobbopusNormalPal[];
extern const u16 gFollowingMonPic_ClodsireNormalPal[];
extern const u16 gFollowingMonPic_CloysterNormalPal[];
extern const u16 gFollowingMonPic_CoalossalNormalPal[];
extern const u16 gFollowingMonPic_CobalionNormalPal[];
extern const u16 gFollowingMonPic_CofagrigusNormalPal[];
extern const u16 gFollowingMonPic_CombeeNormalPal[];
extern const u16 gFollowingMonPic_CombuskenNormalPal[];
extern const u16 gFollowingMonPic_ComfeyNormalPal[];
extern const u16 gFollowingMonPic_ConkeldurrNormalPal[];
extern const u16 gFollowingMonPic_CopperajahNormalPal[];
extern const u16 gFollowingMonPic_CorphishNormalPal[];
extern const u16 gFollowingMonPic_CorsolaNormalPal[];
extern const u16 gFollowingMonPic_CorviknightNormalPal[];
extern const u16 gFollowingMonPic_CorvisquireNormalPal[];
extern const u16 gFollowingMonPic_CosmoemNormalPal[];
extern const u16 gFollowingMonPic_CosmogNormalPal[];
extern const u16 gFollowingMonPic_CottoneeNormalPal[];
extern const u16 gFollowingMonPic_CrabominableNormalPal[];
extern const u16 gFollowingMonPic_CrabrawlerNormalPal[];
extern const u16 gFollowingMonPic_CradilyNormalPal[];
extern const u16 gFollowingMonPic_CramorantNormalPal[];
extern const u16 gFollowingMonPic_CranidosNormalPal[];
extern const u16 gFollowingMonPic_CrawdauntNormalPal[];
extern const u16 gFollowingMonPic_CresseliaNormalPal[];
extern const u16 gFollowingMonPic_CroagunkNormalPal[];
extern const u16 gFollowingMonPic_CrobatNormalPal[];
extern const u16 gFollowingMonPic_CrocalorNormalPal[];
extern const u16 gFollowingMonPic_CroconawNormalPal[];
extern const u16 gFollowingMonPic_CrustleNormalPal[];
extern const u16 gFollowingMonPic_CryogonalNormalPal[];
extern const u16 gFollowingMonPic_CubchooNormalPal[];
extern const u16 gFollowingMonPic_CuboneNormalPal[];
extern const u16 gFollowingMonPic_CufantNormalPal[];
extern const u16 gFollowingMonPic_CursolaNormalPal[];
extern const u16 gFollowingMonPic_CutieflyNormalPal[];
extern const u16 gFollowingMonPic_CyclizarNormalPal[];
extern const u16 gFollowingMonPic_CyndaquilNormalPal[];
extern const u16 gFollowingMonPic_DachsbunNormalPal[];
extern const u16 gFollowingMonPic_DarkraiNormalPal[];
extern const u16 gFollowingMonPic_DarmanitanNormalPal[];
extern const u16 gFollowingMonPic_DartrixNormalPal[];
extern const u16 gFollowingMonPic_DarumakaNormalPal[];
extern const u16 gFollowingMonPic_DecidueyeNormalPal[];
extern const u16 gFollowingMonPic_DedenneNormalPal[];
extern const u16 gFollowingMonPic_DeerlingNormalPal[];
extern const u16 gFollowingMonPic_DeinoNormalPal[];
extern const u16 gFollowingMonPic_DelcattyNormalPal[];
extern const u16 gFollowingMonPic_DelibirdNormalPal[];
extern const u16 gFollowingMonPic_DelphoxNormalPal[];
extern const u16 gFollowingMonPic_DeoxysNormalPal[];
extern const u16 gFollowingMonPic_DewgongNormalPal[];
extern const u16 gFollowingMonPic_DewottNormalPal[];
extern const u16 gFollowingMonPic_DewpiderNormalPal[];
extern const u16 gFollowingMonPic_DhelmiseNormalPal[];
extern const u16 gFollowingMonPic_DialgaNormalPal[];
extern const u16 gFollowingMonPic_DiancieNormalPal[];
extern const u16 gFollowingMonPic_DiggersbyNormalPal[];
extern const u16 gFollowingMonPic_DiglettNormalPal[];
extern const u16 gFollowingMonPic_DipplinNormalPal[];
extern const u16 gFollowingMonPic_DittoNormalPal[];
extern const u16 gFollowingMonPic_DodrioNormalPal[];
extern const u16 gFollowingMonPic_DoduoNormalPal[];
extern const u16 gFollowingMonPic_DollivNormalPal[];
extern const u16 gFollowingMonPic_DondozoNormalPal[];
extern const u16 gFollowingMonPic_DonphanNormalPal[];
extern const u16 gFollowingMonPic_DottlerNormalPal[];
extern const u16 gFollowingMonPic_DoubladeNormalPal[];
extern const u16 gFollowingMonPic_DracovishNormalPal[];
extern const u16 gFollowingMonPic_DracozoltNormalPal[];
extern const u16 gFollowingMonPic_DragalgeNormalPal[];
extern const u16 gFollowingMonPic_DragapultNormalPal[];
extern const u16 gFollowingMonPic_DragonairNormalPal[];
extern const u16 gFollowingMonPic_DragoniteNormalPal[];
extern const u16 gFollowingMonPic_DrakloakNormalPal[];
extern const u16 gFollowingMonPic_DrampaNormalPal[];
extern const u16 gFollowingMonPic_DrapionNormalPal[];
extern const u16 gFollowingMonPic_DratiniNormalPal[];
extern const u16 gFollowingMonPic_DrednawNormalPal[];
extern const u16 gFollowingMonPic_DreepyNormalPal[];
extern const u16 gFollowingMonPic_DrifblimNormalPal[];
extern const u16 gFollowingMonPic_DrifloonNormalPal[];
extern const u16 gFollowingMonPic_DrilburNormalPal[];
extern const u16 gFollowingMonPic_DrizzileNormalPal[];
extern const u16 gFollowingMonPic_DrowzeeNormalPal[];
extern const u16 gFollowingMonPic_DruddigonNormalPal[];
extern const u16 gFollowingMonPic_DubwoolNormalPal[];
extern const u16 gFollowingMonPic_DucklettNormalPal[];
extern const u16 gFollowingMonPic_DudunsparceNormalPal[];
extern const u16 gFollowingMonPic_DugtrioNormalPal[];
extern const u16 gFollowingMonPic_DunsparceNormalPal[];
extern const u16 gFollowingMonPic_DuosionNormalPal[];
extern const u16 gFollowingMonPic_DuraludonNormalPal[];
extern const u16 gFollowingMonPic_DurantNormalPal[];
extern const u16 gFollowingMonPic_DusclopsNormalPal[];
extern const u16 gFollowingMonPic_DusknoirNormalPal[];
extern const u16 gFollowingMonPic_DuskullNormalPal[];
extern const u16 gFollowingMonPic_DustoxNormalPal[];
extern const u16 gFollowingMonPic_DwebbleNormalPal[];
extern const u16 gFollowingMonPic_EelektrikNormalPal[];
extern const u16 gFollowingMonPic_EelektrossNormalPal[];
extern const u16 gFollowingMonPic_EeveeNormalPal[];
extern const u16 gFollowingMonPic_EiscueNormalPal[];
extern const u16 gFollowingMonPic_EkansNormalPal[];
extern const u16 gFollowingMonPic_EldegossNormalPal[];
extern const u16 gFollowingMonPic_ElectabuzzNormalPal[];
extern const u16 gFollowingMonPic_ElectivireNormalPal[];
extern const u16 gFollowingMonPic_ElectrikeNormalPal[];
extern const u16 gFollowingMonPic_ElectrodeNormalPal[];
extern const u16 gFollowingMonPic_ElekidNormalPal[];
extern const u16 gFollowingMonPic_ElgyemNormalPal[];
extern const u16 gFollowingMonPic_EmboarNormalPal[];
extern const u16 gFollowingMonPic_EmolgaNormalPal[];
extern const u16 gFollowingMonPic_EmpoleonNormalPal[];
extern const u16 gFollowingMonPic_EnamorusNormalPal[];
extern const u16 gFollowingMonPic_EnteiNormalPal[];
extern const u16 gFollowingMonPic_EscavalierNormalPal[];
extern const u16 gFollowingMonPic_EspathraNormalPal[];
extern const u16 gFollowingMonPic_EspeonNormalPal[];
extern const u16 gFollowingMonPic_EspurrNormalPal[];
extern const u16 gFollowingMonPic_EternatusNormalPal[];
extern const u16 gFollowingMonPic_ExcadrillNormalPal[];
extern const u16 gFollowingMonPic_ExeggcuteNormalPal[];
extern const u16 gFollowingMonPic_ExeggutorNormalPal[];
extern const u16 gFollowingMonPic_ExploudNormalPal[];
extern const u16 gFollowingMonPic_FalinksNormalPal[];
extern const u16 gFollowingMonPic_FarfetchdNormalPal[];
extern const u16 gFollowingMonPic_FarigirafNormalPal[];
extern const u16 gFollowingMonPic_FearowNormalPal[];
extern const u16 gFollowingMonPic_FeebasNormalPal[];
extern const u16 gFollowingMonPic_FennekinNormalPal[];
extern const u16 gFollowingMonPic_FeraligatrNormalPal[];
extern const u16 gFollowingMonPic_FerroseedNormalPal[];
extern const u16 gFollowingMonPic_FerrothornNormalPal[];
extern const u16 gFollowingMonPic_FezandipitiNormalPal[];
extern const u16 gFollowingMonPic_FidoughNormalPal[];
extern const u16 gFollowingMonPic_FinizenNormalPal[];
extern const u16 gFollowingMonPic_FinneonNormalPal[];
extern const u16 gFollowingMonPic_FlaaffyNormalPal[];
extern const u16 gFollowingMonPic_FlabebeNormalPal[];
extern const u16 gFollowingMonPic_FlamigoNormalPal[];
extern const u16 gFollowingMonPic_FlappleNormalPal[];
extern const u16 gFollowingMonPic_FlareonNormalPal[];
extern const u16 gFollowingMonPic_FletchinderNormalPal[];
extern const u16 gFollowingMonPic_FletchlingNormalPal[];
extern const u16 gFollowingMonPic_FlittleNormalPal[];
extern const u16 gFollowingMonPic_FloatzelNormalPal[];
extern const u16 gFollowingMonPic_FloetteNormalPal[];
extern const u16 gFollowingMonPic_FloragatoNormalPal[];
extern const u16 gFollowingMonPic_FlorgesNormalPal[];
extern const u16 gFollowingMonPic_Flutter_maneNormalPal[];
extern const u16 gFollowingMonPic_FlygonNormalPal[];
extern const u16 gFollowingMonPic_FomantisNormalPal[];
extern const u16 gFollowingMonPic_FoongusNormalPal[];
extern const u16 gFollowingMonPic_ForretressNormalPal[];
extern const u16 gFollowingMonPic_FraxureNormalPal[];
extern const u16 gFollowingMonPic_FrigibaxNormalPal[];
extern const u16 gFollowingMonPic_FrillishNormalPal[];
extern const u16 gFollowingMonPic_FroakieNormalPal[];
extern const u16 gFollowingMonPic_FrogadierNormalPal[];
extern const u16 gFollowingMonPic_FroslassNormalPal[];
extern const u16 gFollowingMonPic_FrosmothNormalPal[];
extern const u16 gFollowingMonPic_FuecocoNormalPal[];
extern const u16 gFollowingMonPic_FurfrouNormalPal[];
extern const u16 gFollowingMonPic_FurretNormalPal[];
extern const u16 gFollowingMonPic_GabiteNormalPal[];
extern const u16 gFollowingMonPic_GalladeNormalPal[];
extern const u16 gFollowingMonPic_GalvantulaNormalPal[];
extern const u16 gFollowingMonPic_GarbodorNormalPal[];
extern const u16 gFollowingMonPic_GarchompNormalPal[];
extern const u16 gFollowingMonPic_GardevoirNormalPal[];
extern const u16 gFollowingMonPic_GarganaclNormalPal[];
extern const u16 gFollowingMonPic_GastlyNormalPal[];
extern const u16 gFollowingMonPic_GastrodonNormalPal[];
extern const u16 gFollowingMonPic_GenesectNormalPal[];
extern const u16 gFollowingMonPic_GengarNormalPal[];
extern const u16 gFollowingMonPic_GeodudeNormalPal[];
extern const u16 gFollowingMonPic_GholdengoNormalPal[];
extern const u16 gFollowingMonPic_GibleNormalPal[];
extern const u16 gFollowingMonPic_GigalithNormalPal[];
extern const u16 gFollowingMonPic_GimmighoulNormalPal[];
extern const u16 gFollowingMonPic_GirafarigNormalPal[];
extern const u16 gFollowingMonPic_GiratinaNormalPal[];
extern const u16 gFollowingMonPic_GlaceonNormalPal[];
extern const u16 gFollowingMonPic_GlalieNormalPal[];
extern const u16 gFollowingMonPic_GlameowNormalPal[];
extern const u16 gFollowingMonPic_GlastrierNormalPal[];
extern const u16 gFollowingMonPic_GligarNormalPal[];
extern const u16 gFollowingMonPic_GlimmetNormalPal[];
extern const u16 gFollowingMonPic_GlimmoraNormalPal[];
extern const u16 gFollowingMonPic_GliscorNormalPal[];
extern const u16 gFollowingMonPic_GloomNormalPal[];
extern const u16 gFollowingMonPic_GogoatNormalPal[];
extern const u16 gFollowingMonPic_GolbatNormalPal[];
extern const u16 gFollowingMonPic_GoldeenNormalPal[];
extern const u16 gFollowingMonPic_GolduckNormalPal[];
extern const u16 gFollowingMonPic_GolemNormalPal[];
extern const u16 gFollowingMonPic_GolettNormalPal[];
extern const u16 gFollowingMonPic_GolisopodNormalPal[];
extern const u16 gFollowingMonPic_GolurkNormalPal[];
extern const u16 gFollowingMonPic_GoodraNormalPal[];
extern const u16 gFollowingMonPic_GoomyNormalPal[];
extern const u16 gFollowingMonPic_GorebyssNormalPal[];
extern const u16 gFollowingMonPic_GossifleurNormalPal[];
extern const u16 gFollowingMonPic_GothitaNormalPal[];
extern const u16 gFollowingMonPic_GothitelleNormalPal[];
extern const u16 gFollowingMonPic_GothoritaNormalPal[];
extern const u16 gFollowingMonPic_Gouging_fireNormalPal[];
extern const u16 gFollowingMonPic_GourgeistNormalPal[];
extern const u16 gFollowingMonPic_GrafaiaiNormalPal[];
extern const u16 gFollowingMonPic_GranbullNormalPal[];
extern const u16 gFollowingMonPic_GrapploctNormalPal[];
extern const u16 gFollowingMonPic_GravelerNormalPal[];
extern const u16 gFollowingMonPic_Great_tuskNormalPal[];
extern const u16 gFollowingMonPic_GreavardNormalPal[];
extern const u16 gFollowingMonPic_GreedentNormalPal[];
extern const u16 gFollowingMonPic_GreninjaNormalPal[];
extern const u16 gFollowingMonPic_GrimerNormalPal[];
extern const u16 gFollowingMonPic_GrimmsnarlNormalPal[];
extern const u16 gFollowingMonPic_GrookeyNormalPal[];
extern const u16 gFollowingMonPic_GrotleNormalPal[];
extern const u16 gFollowingMonPic_GroudonNormalPal[];
extern const u16 gFollowingMonPic_GrovyleNormalPal[];
extern const u16 gFollowingMonPic_GrowlitheNormalPal[];
extern const u16 gFollowingMonPic_GrubbinNormalPal[];
extern const u16 gFollowingMonPic_GrumpigNormalPal[];
extern const u16 gFollowingMonPic_GulpinNormalPal[];
extern const u16 gFollowingMonPic_GumshoosNormalPal[];
extern const u16 gFollowingMonPic_GurdurrNormalPal[];
extern const u16 gFollowingMonPic_GuzzlordNormalPal[];
extern const u16 gFollowingMonPic_GyaradosNormalPal[];
extern const u16 gFollowingMonPic_Hakamo_oNormalPal[];
extern const u16 gFollowingMonPic_HappinyNormalPal[];
extern const u16 gFollowingMonPic_HariyamaNormalPal[];
extern const u16 gFollowingMonPic_HatennaNormalPal[];
extern const u16 gFollowingMonPic_HattereneNormalPal[];
extern const u16 gFollowingMonPic_HattremNormalPal[];
extern const u16 gFollowingMonPic_HaunterNormalPal[];
extern const u16 gFollowingMonPic_HawluchaNormalPal[];
extern const u16 gFollowingMonPic_HaxorusNormalPal[];
extern const u16 gFollowingMonPic_HeatmorNormalPal[];
extern const u16 gFollowingMonPic_HeatranNormalPal[];
extern const u16 gFollowingMonPic_HelioliskNormalPal[];
extern const u16 gFollowingMonPic_HelioptileNormalPal[];
extern const u16 gFollowingMonPic_HeracrossNormalPal[];
extern const u16 gFollowingMonPic_HerdierNormalPal[];
extern const u16 gFollowingMonPic_HippopotasNormalPal[];
extern const u16 gFollowingMonPic_HippowdonNormalPal[];
extern const u16 gFollowingMonPic_HitmonchanNormalPal[];
extern const u16 gFollowingMonPic_HitmonleeNormalPal[];
extern const u16 gFollowingMonPic_HitmontopNormalPal[];
extern const u16 gFollowingMonPic_HonchkrowNormalPal[];
extern const u16 gFollowingMonPic_HonedgeNormalPal[];
extern const u16 gFollowingMonPic_HoopaNormalPal[];
extern const u16 gFollowingMonPic_HoothootNormalPal[];
extern const u16 gFollowingMonPic_HoppipNormalPal[];
extern const u16 gFollowingMonPic_HorseaNormalPal[];
extern const u16 gFollowingMonPic_HoundoomNormalPal[];
extern const u16 gFollowingMonPic_HoundourNormalPal[];
extern const u16 gFollowingMonPic_HoundstoneNormalPal[];
extern const u16 gFollowingMonPic_Ho_ohNormalPal[];
extern const u16 gFollowingMonPic_HuntailNormalPal[];
extern const u16 gFollowingMonPic_HydrappleNormalPal[];
extern const u16 gFollowingMonPic_HydreigonNormalPal[];
extern const u16 gFollowingMonPic_HypnoNormalPal[];
extern const u16 gFollowingMonPic_IgglybuffNormalPal[];
extern const u16 gFollowingMonPic_IllumiseNormalPal[];
extern const u16 gFollowingMonPic_ImpidimpNormalPal[];
extern const u16 gFollowingMonPic_IncineroarNormalPal[];
extern const u16 gFollowingMonPic_IndeedeeNormalPal[];
extern const u16 gFollowingMonPic_InfernapeNormalPal[];
extern const u16 gFollowingMonPic_InkayNormalPal[];
extern const u16 gFollowingMonPic_InteleonNormalPal[];
extern const u16 gFollowingMonPic_Iron_boulderNormalPal[];
extern const u16 gFollowingMonPic_Iron_bundleNormalPal[];
extern const u16 gFollowingMonPic_Iron_crownNormalPal[];
extern const u16 gFollowingMonPic_Iron_handsNormalPal[];
extern const u16 gFollowingMonPic_Iron_jugulisNormalPal[];
extern const u16 gFollowingMonPic_Iron_leavesNormalPal[];
extern const u16 gFollowingMonPic_Iron_mothNormalPal[];
extern const u16 gFollowingMonPic_Iron_thornsNormalPal[];
extern const u16 gFollowingMonPic_Iron_treadsNormalPal[];
extern const u16 gFollowingMonPic_Iron_valiantNormalPal[];
extern const u16 gFollowingMonPic_IvysaurNormalPal[];
extern const u16 gFollowingMonPic_Jangmo_oNormalPal[];
extern const u16 gFollowingMonPic_JellicentNormalPal[];
extern const u16 gFollowingMonPic_JigglypuffNormalPal[];
extern const u16 gFollowingMonPic_JirachiNormalPal[];
extern const u16 gFollowingMonPic_JolteonNormalPal[];
extern const u16 gFollowingMonPic_JoltikNormalPal[];
extern const u16 gFollowingMonPic_JumpluffNormalPal[];
extern const u16 gFollowingMonPic_JynxNormalPal[];
extern const u16 gFollowingMonPic_KabutoNormalPal[];
extern const u16 gFollowingMonPic_KabutopsNormalPal[];
extern const u16 gFollowingMonPic_KadabraNormalPal[];
extern const u16 gFollowingMonPic_KakunaNormalPal[];
extern const u16 gFollowingMonPic_KangaskhanNormalPal[];
extern const u16 gFollowingMonPic_KarrablastNormalPal[];
extern const u16 gFollowingMonPic_KartanaNormalPal[];
extern const u16 gFollowingMonPic_KecleonNormalPal[];
extern const u16 gFollowingMonPic_KeldeoNormalPal[];
extern const u16 gFollowingMonPic_KilowattrelNormalPal[];
extern const u16 gFollowingMonPic_KingambitNormalPal[];
extern const u16 gFollowingMonPic_KingdraNormalPal[];
extern const u16 gFollowingMonPic_KinglerNormalPal[];
extern const u16 gFollowingMonPic_KirliaNormalPal[];
extern const u16 gFollowingMonPic_KlangNormalPal[];
extern const u16 gFollowingMonPic_KlawfNormalPal[];
extern const u16 gFollowingMonPic_KleavorNormalPal[];
extern const u16 gFollowingMonPic_KlefkiNormalPal[];
extern const u16 gFollowingMonPic_KlinkNormalPal[];
extern const u16 gFollowingMonPic_KlinklangNormalPal[];
extern const u16 gFollowingMonPic_KoffingNormalPal[];
extern const u16 gFollowingMonPic_KomalaNormalPal[];
extern const u16 gFollowingMonPic_Kommo_oNormalPal[];
extern const u16 gFollowingMonPic_KoraidonNormalPal[];
extern const u16 gFollowingMonPic_KrabbyNormalPal[];
extern const u16 gFollowingMonPic_KricketotNormalPal[];
extern const u16 gFollowingMonPic_KricketuneNormalPal[];
extern const u16 gFollowingMonPic_KrokorokNormalPal[];
extern const u16 gFollowingMonPic_KrookodileNormalPal[];
extern const u16 gFollowingMonPic_KubfuNormalPal[];
extern const u16 gFollowingMonPic_KyogreNormalPal[];
extern const u16 gFollowingMonPic_KyuremNormalPal[];
extern const u16 gFollowingMonPic_LaironNormalPal[];
extern const u16 gFollowingMonPic_LampentNormalPal[];
extern const u16 gFollowingMonPic_LandorusNormalPal[];
extern const u16 gFollowingMonPic_LanturnNormalPal[];
extern const u16 gFollowingMonPic_LaprasNormalPal[];
extern const u16 gFollowingMonPic_LarvestaNormalPal[];
extern const u16 gFollowingMonPic_LarvitarNormalPal[];
extern const u16 gFollowingMonPic_LatiasNormalPal[];
extern const u16 gFollowingMonPic_LatiosNormalPal[];
extern const u16 gFollowingMonPic_LeafeonNormalPal[];
extern const u16 gFollowingMonPic_LeavannyNormalPal[];
extern const u16 gFollowingMonPic_LechonkNormalPal[];
extern const u16 gFollowingMonPic_LedianNormalPal[];
extern const u16 gFollowingMonPic_LedybaNormalPal[];
extern const u16 gFollowingMonPic_LickilickyNormalPal[];
extern const u16 gFollowingMonPic_LickitungNormalPal[];
extern const u16 gFollowingMonPic_LiepardNormalPal[];
extern const u16 gFollowingMonPic_LileepNormalPal[];
extern const u16 gFollowingMonPic_LilligantNormalPal[];
extern const u16 gFollowingMonPic_LillipupNormalPal[];
extern const u16 gFollowingMonPic_LinooneNormalPal[];
extern const u16 gFollowingMonPic_LitleoNormalPal[];
extern const u16 gFollowingMonPic_LittenNormalPal[];
extern const u16 gFollowingMonPic_LitwickNormalPal[];
extern const u16 gFollowingMonPic_LokixNormalPal[];
extern const u16 gFollowingMonPic_LombreNormalPal[];
extern const u16 gFollowingMonPic_LopunnyNormalPal[];
extern const u16 gFollowingMonPic_LotadNormalPal[];
extern const u16 gFollowingMonPic_LoudredNormalPal[];
extern const u16 gFollowingMonPic_LucarioNormalPal[];
extern const u16 gFollowingMonPic_LudicoloNormalPal[];
extern const u16 gFollowingMonPic_LugiaNormalPal[];
extern const u16 gFollowingMonPic_LumineonNormalPal[];
extern const u16 gFollowingMonPic_LunalaNormalPal[];
extern const u16 gFollowingMonPic_LunatoneNormalPal[];
extern const u16 gFollowingMonPic_LurantisNormalPal[];
extern const u16 gFollowingMonPic_LuvdiscNormalPal[];
extern const u16 gFollowingMonPic_LuxioNormalPal[];
extern const u16 gFollowingMonPic_LuxrayNormalPal[];
extern const u16 gFollowingMonPic_LycanrocNormalPal[];
extern const u16 gFollowingMonPic_MabosstiffNormalPal[];
extern const u16 gFollowingMonPic_MachampNormalPal[];
extern const u16 gFollowingMonPic_MachokeNormalPal[];
extern const u16 gFollowingMonPic_MachopNormalPal[];
extern const u16 gFollowingMonPic_MagbyNormalPal[];
extern const u16 gFollowingMonPic_MagcargoNormalPal[];
extern const u16 gFollowingMonPic_MagearnaNormalPal[];
extern const u16 gFollowingMonPic_MagikarpNormalPal[];
extern const u16 gFollowingMonPic_MagmarNormalPal[];
extern const u16 gFollowingMonPic_MagmortarNormalPal[];
extern const u16 gFollowingMonPic_MagnemiteNormalPal[];
extern const u16 gFollowingMonPic_MagnetonNormalPal[];
extern const u16 gFollowingMonPic_MagnezoneNormalPal[];
extern const u16 gFollowingMonPic_MakuhitaNormalPal[];
extern const u16 gFollowingMonPic_MalamarNormalPal[];
extern const u16 gFollowingMonPic_MamoswineNormalPal[];
extern const u16 gFollowingMonPic_ManaphyNormalPal[];
extern const u16 gFollowingMonPic_MandibuzzNormalPal[];
extern const u16 gFollowingMonPic_ManectricNormalPal[];
extern const u16 gFollowingMonPic_MankeyNormalPal[];
extern const u16 gFollowingMonPic_MantineNormalPal[];
extern const u16 gFollowingMonPic_MantykeNormalPal[];
extern const u16 gFollowingMonPic_MaractusNormalPal[];
extern const u16 gFollowingMonPic_MareanieNormalPal[];
extern const u16 gFollowingMonPic_MareepNormalPal[];
extern const u16 gFollowingMonPic_MarillNormalPal[];
extern const u16 gFollowingMonPic_MarowakNormalPal[];
extern const u16 gFollowingMonPic_MarshadowNormalPal[];
extern const u16 gFollowingMonPic_MarshtompNormalPal[];
extern const u16 gFollowingMonPic_MaschiffNormalPal[];
extern const u16 gFollowingMonPic_MasquerainNormalPal[];
extern const u16 gFollowingMonPic_MausholdNormalPal[];
extern const u16 gFollowingMonPic_MawileNormalPal[];
extern const u16 gFollowingMonPic_MedichamNormalPal[];
extern const u16 gFollowingMonPic_MedititeNormalPal[];
extern const u16 gFollowingMonPic_MeganiumNormalPal[];
extern const u16 gFollowingMonPic_MelmetalNormalPal[];
extern const u16 gFollowingMonPic_MeloettaNormalPal[];
extern const u16 gFollowingMonPic_MeltanNormalPal[];
extern const u16 gFollowingMonPic_MeowscaradaNormalPal[];
extern const u16 gFollowingMonPic_MeowsticNormalPal[];
extern const u16 gFollowingMonPic_MeowthNormalPal[];
extern const u16 gFollowingMonPic_MespritNormalPal[];
extern const u16 gFollowingMonPic_MetagrossNormalPal[];
extern const u16 gFollowingMonPic_MetangNormalPal[];
extern const u16 gFollowingMonPic_MetapodNormalPal[];
extern const u16 gFollowingMonPic_MewNormalPal[];
extern const u16 gFollowingMonPic_MewtwoNormalPal[];
extern const u16 gFollowingMonPic_MienfooNormalPal[];
extern const u16 gFollowingMonPic_MienshaoNormalPal[];
extern const u16 gFollowingMonPic_MightyenaNormalPal[];
extern const u16 gFollowingMonPic_MilceryNormalPal[];
extern const u16 gFollowingMonPic_MiloticNormalPal[];
extern const u16 gFollowingMonPic_MiltankNormalPal[];
extern const u16 gFollowingMonPic_Mime_jrNormalPal[];
extern const u16 gFollowingMonPic_MimikyuNormalPal[];
extern const u16 gFollowingMonPic_MinccinoNormalPal[];
extern const u16 gFollowingMonPic_MiniorNormalPal[];
extern const u16 gFollowingMonPic_MinunNormalPal[];
extern const u16 gFollowingMonPic_MiraidonNormalPal[];
extern const u16 gFollowingMonPic_MisdreavusNormalPal[];
extern const u16 gFollowingMonPic_MismagiusNormalPal[];
extern const u16 gFollowingMonPic_MoltresNormalPal[];
extern const u16 gFollowingMonPic_MonfernoNormalPal[];
extern const u16 gFollowingMonPic_MorelullNormalPal[];
extern const u16 gFollowingMonPic_MorgremNormalPal[];
extern const u16 gFollowingMonPic_MorpekoNormalPal[];
extern const u16 gFollowingMonPic_MothimNormalPal[];
extern const u16 gFollowingMonPic_Mr_mimeNormalPal[];
extern const u16 gFollowingMonPic_Mr_rimeNormalPal[];
extern const u16 gFollowingMonPic_MudbrayNormalPal[];
extern const u16 gFollowingMonPic_MudkipNormalPal[];
extern const u16 gFollowingMonPic_MudsdaleNormalPal[];
extern const u16 gFollowingMonPic_MukNormalPal[];
extern const u16 gFollowingMonPic_MunchlaxNormalPal[];
extern const u16 gFollowingMonPic_MunkidoriNormalPal[];
extern const u16 gFollowingMonPic_MunnaNormalPal[];
extern const u16 gFollowingMonPic_MurkrowNormalPal[];
extern const u16 gFollowingMonPic_MusharnaNormalPal[];
extern const u16 gFollowingMonPic_NacliNormalPal[];
extern const u16 gFollowingMonPic_NaclstackNormalPal[];
extern const u16 gFollowingMonPic_NaganadelNormalPal[];
extern const u16 gFollowingMonPic_NatuNormalPal[];
extern const u16 gFollowingMonPic_NecrozmaNormalPal[];
extern const u16 gFollowingMonPic_NickitNormalPal[];
extern const u16 gFollowingMonPic_NidokingNormalPal[];
extern const u16 gFollowingMonPic_NidoqueenNormalPal[];
extern const u16 gFollowingMonPic_Nidoran_fNormalPal[];
extern const u16 gFollowingMonPic_Nidoran_mNormalPal[];
extern const u16 gFollowingMonPic_NidorinaNormalPal[];
extern const u16 gFollowingMonPic_NidorinoNormalPal[];
extern const u16 gFollowingMonPic_NihilegoNormalPal[];
extern const u16 gFollowingMonPic_NincadaNormalPal[];
extern const u16 gFollowingMonPic_NinetalesNormalPal[];
extern const u16 gFollowingMonPic_NinjaskNormalPal[];
extern const u16 gFollowingMonPic_NoctowlNormalPal[];
extern const u16 gFollowingMonPic_NoibatNormalPal[];
extern const u16 gFollowingMonPic_NoivernNormalPal[];
extern const u16 gFollowingMonPic_NosepassNormalPal[];
extern const u16 gFollowingMonPic_NumelNormalPal[];
extern const u16 gFollowingMonPic_NuzleafNormalPal[];
extern const u16 gFollowingMonPic_NymbleNormalPal[];
extern const u16 gFollowingMonPic_ObstagoonNormalPal[];
extern const u16 gFollowingMonPic_OctilleryNormalPal[];
extern const u16 gFollowingMonPic_OddishNormalPal[];
extern const u16 gFollowingMonPic_OgerponNormalPal[];
extern const u16 gFollowingMonPic_OinkologneNormalPal[];
extern const u16 gFollowingMonPic_OkidogiNormalPal[];
extern const u16 gFollowingMonPic_OmanyteNormalPal[];
extern const u16 gFollowingMonPic_OmastarNormalPal[];
extern const u16 gFollowingMonPic_OnixNormalPal[];
extern const u16 gFollowingMonPic_OranguruNormalPal[];
extern const u16 gFollowingMonPic_OrbeetleNormalPal[];
extern const u16 gFollowingMonPic_OricorioNormalPal[];
extern const u16 gFollowingMonPic_OrthwormNormalPal[];
extern const u16 gFollowingMonPic_OshawottNormalPal[];
extern const u16 gFollowingMonPic_OverqwilNormalPal[];
extern const u16 gFollowingMonPic_PachirisuNormalPal[];
extern const u16 gFollowingMonPic_PalafinNormalPal[];
extern const u16 gFollowingMonPic_PalkiaNormalPal[];
extern const u16 gFollowingMonPic_PalossandNormalPal[];
extern const u16 gFollowingMonPic_PalpitoadNormalPal[];
extern const u16 gFollowingMonPic_PanchamNormalPal[];
extern const u16 gFollowingMonPic_PangoroNormalPal[];
extern const u16 gFollowingMonPic_PanpourNormalPal[];
extern const u16 gFollowingMonPic_PansageNormalPal[];
extern const u16 gFollowingMonPic_PansearNormalPal[];
extern const u16 gFollowingMonPic_ParasNormalPal[];
extern const u16 gFollowingMonPic_ParasectNormalPal[];
extern const u16 gFollowingMonPic_PassimianNormalPal[];
extern const u16 gFollowingMonPic_PatratNormalPal[];
extern const u16 gFollowingMonPic_PawmiNormalPal[];
extern const u16 gFollowingMonPic_PawmoNormalPal[];
extern const u16 gFollowingMonPic_PawmotNormalPal[];
extern const u16 gFollowingMonPic_PawniardNormalPal[];
extern const u16 gFollowingMonPic_PecharuntNormalPal[];
extern const u16 gFollowingMonPic_PelipperNormalPal[];
extern const u16 gFollowingMonPic_PerrserkerNormalPal[];
extern const u16 gFollowingMonPic_PersianNormalPal[];
extern const u16 gFollowingMonPic_PetililNormalPal[];
extern const u16 gFollowingMonPic_PhanpyNormalPal[];
extern const u16 gFollowingMonPic_PhantumpNormalPal[];
extern const u16 gFollowingMonPic_PheromosaNormalPal[];
extern const u16 gFollowingMonPic_PhioneNormalPal[];
extern const u16 gFollowingMonPic_PichuNormalPal[];
extern const u16 gFollowingMonPic_PidgeotNormalPal[];
extern const u16 gFollowingMonPic_PidgeottoNormalPal[];
extern const u16 gFollowingMonPic_PidgeyNormalPal[];
extern const u16 gFollowingMonPic_PidoveNormalPal[];
extern const u16 gFollowingMonPic_PigniteNormalPal[];
extern const u16 gFollowingMonPic_PikachuNormalPal[];
extern const u16 gFollowingMonPic_PikipekNormalPal[];
extern const u16 gFollowingMonPic_PiloswineNormalPal[];
extern const u16 gFollowingMonPic_PincurchinNormalPal[];
extern const u16 gFollowingMonPic_PinecoNormalPal[];
extern const u16 gFollowingMonPic_PinsirNormalPal[];
extern const u16 gFollowingMonPic_PiplupNormalPal[];
extern const u16 gFollowingMonPic_PlusleNormalPal[];
extern const u16 gFollowingMonPic_PoipoleNormalPal[];
extern const u16 gFollowingMonPic_PolitoedNormalPal[];
extern const u16 gFollowingMonPic_PoliwagNormalPal[];
extern const u16 gFollowingMonPic_PoliwhirlNormalPal[];
extern const u16 gFollowingMonPic_PoliwrathNormalPal[];
extern const u16 gFollowingMonPic_PoltchageistNormalPal[];
extern const u16 gFollowingMonPic_PolteageistNormalPal[];
extern const u16 gFollowingMonPic_PonytaNormalPal[];
extern const u16 gFollowingMonPic_PoochyenaNormalPal[];
extern const u16 gFollowingMonPic_PopplioNormalPal[];
extern const u16 gFollowingMonPic_PorygonNormalPal[];
extern const u16 gFollowingMonPic_Porygon2NormalPal[];
extern const u16 gFollowingMonPic_Porygon_zNormalPal[];
extern const u16 gFollowingMonPic_PrimarinaNormalPal[];
extern const u16 gFollowingMonPic_PrimeapeNormalPal[];
extern const u16 gFollowingMonPic_PrinplupNormalPal[];
extern const u16 gFollowingMonPic_ProbopassNormalPal[];
extern const u16 gFollowingMonPic_PsyduckNormalPal[];
extern const u16 gFollowingMonPic_PumpkabooNormalPal[];
extern const u16 gFollowingMonPic_PupitarNormalPal[];
extern const u16 gFollowingMonPic_PurrloinNormalPal[];
extern const u16 gFollowingMonPic_PuruglyNormalPal[];
extern const u16 gFollowingMonPic_PyroarNormalPal[];
extern const u16 gFollowingMonPic_PyukumukuNormalPal[];
extern const u16 gFollowingMonPic_QuagsireNormalPal[];
extern const u16 gFollowingMonPic_QuaquavalNormalPal[];
extern const u16 gFollowingMonPic_QuaxlyNormalPal[];
extern const u16 gFollowingMonPic_QuaxwellNormalPal[];
extern const u16 gFollowingMonPic_QuilavaNormalPal[];
extern const u16 gFollowingMonPic_QuilladinNormalPal[];
extern const u16 gFollowingMonPic_QwilfishNormalPal[];
extern const u16 gFollowingMonPic_RabootNormalPal[];
extern const u16 gFollowingMonPic_RabscaNormalPal[];
extern const u16 gFollowingMonPic_Raging_boltNormalPal[];
extern const u16 gFollowingMonPic_RaichuNormalPal[];
extern const u16 gFollowingMonPic_RaikouNormalPal[];
extern const u16 gFollowingMonPic_RaltsNormalPal[];
extern const u16 gFollowingMonPic_RampardosNormalPal[];
extern const u16 gFollowingMonPic_RapidashNormalPal[];
extern const u16 gFollowingMonPic_RaticateNormalPal[];
extern const u16 gFollowingMonPic_RattataNormalPal[];
extern const u16 gFollowingMonPic_RayquazaNormalPal[];
extern const u16 gFollowingMonPic_RegiceNormalPal[];
extern const u16 gFollowingMonPic_RegidragoNormalPal[];
extern const u16 gFollowingMonPic_RegielekiNormalPal[];
extern const u16 gFollowingMonPic_RegigigasNormalPal[];
extern const u16 gFollowingMonPic_RegirockNormalPal[];
extern const u16 gFollowingMonPic_RegisteelNormalPal[];
extern const u16 gFollowingMonPic_RelicanthNormalPal[];
extern const u16 gFollowingMonPic_RellorNormalPal[];
extern const u16 gFollowingMonPic_RemoraidNormalPal[];
extern const u16 gFollowingMonPic_ReshiramNormalPal[];
extern const u16 gFollowingMonPic_ReuniclusNormalPal[];
extern const u16 gFollowingMonPic_RevavroomNormalPal[];
extern const u16 gFollowingMonPic_RhydonNormalPal[];
extern const u16 gFollowingMonPic_RhyhornNormalPal[];
extern const u16 gFollowingMonPic_RhyperiorNormalPal[];
extern const u16 gFollowingMonPic_RibombeeNormalPal[];
extern const u16 gFollowingMonPic_RillaboomNormalPal[];
extern const u16 gFollowingMonPic_RioluNormalPal[];
extern const u16 gFollowingMonPic_Roaring_moonNormalPal[];
extern const u16 gFollowingMonPic_RockruffNormalPal[];
extern const u16 gFollowingMonPic_RoggenrolaNormalPal[];
extern const u16 gFollowingMonPic_RolycolyNormalPal[];
extern const u16 gFollowingMonPic_RookideeNormalPal[];
extern const u16 gFollowingMonPic_RoseliaNormalPal[];
extern const u16 gFollowingMonPic_RoseradeNormalPal[];
extern const u16 gFollowingMonPic_RotomNormalPal[];
extern const u16 gFollowingMonPic_RowletNormalPal[];
extern const u16 gFollowingMonPic_RuffletNormalPal[];
extern const u16 gFollowingMonPic_RunerigusNormalPal[];
extern const u16 gFollowingMonPic_SableyeNormalPal[];
extern const u16 gFollowingMonPic_SalamenceNormalPal[];
extern const u16 gFollowingMonPic_SalanditNormalPal[];
extern const u16 gFollowingMonPic_SalazzleNormalPal[];
extern const u16 gFollowingMonPic_SamurottNormalPal[];
extern const u16 gFollowingMonPic_SandacondaNormalPal[];
extern const u16 gFollowingMonPic_SandileNormalPal[];
extern const u16 gFollowingMonPic_SandshrewNormalPal[];
extern const u16 gFollowingMonPic_SandslashNormalPal[];
extern const u16 gFollowingMonPic_SandygastNormalPal[];
extern const u16 gFollowingMonPic_Sandy_shocksNormalPal[];
extern const u16 gFollowingMonPic_SawkNormalPal[];
extern const u16 gFollowingMonPic_SawsbuckNormalPal[];
extern const u16 gFollowingMonPic_ScatterbugNormalPal[];
extern const u16 gFollowingMonPic_SceptileNormalPal[];
extern const u16 gFollowingMonPic_ScizorNormalPal[];
extern const u16 gFollowingMonPic_ScolipedeNormalPal[];
extern const u16 gFollowingMonPic_ScorbunnyNormalPal[];
extern const u16 gFollowingMonPic_ScovillainNormalPal[];
extern const u16 gFollowingMonPic_ScraftyNormalPal[];
extern const u16 gFollowingMonPic_ScraggyNormalPal[];
extern const u16 gFollowingMonPic_Scream_tailNormalPal[];
extern const u16 gFollowingMonPic_ScytherNormalPal[];
extern const u16 gFollowingMonPic_SeadraNormalPal[];
extern const u16 gFollowingMonPic_SeakingNormalPal[];
extern const u16 gFollowingMonPic_SealeoNormalPal[];
extern const u16 gFollowingMonPic_SeedotNormalPal[];
extern const u16 gFollowingMonPic_SeelNormalPal[];
extern const u16 gFollowingMonPic_SeismitoadNormalPal[];
extern const u16 gFollowingMonPic_SentretNormalPal[];
extern const u16 gFollowingMonPic_SerperiorNormalPal[];
extern const u16 gFollowingMonPic_ServineNormalPal[];
extern const u16 gFollowingMonPic_SeviperNormalPal[];
extern const u16 gFollowingMonPic_SewaddleNormalPal[];
extern const u16 gFollowingMonPic_SharpedoNormalPal[];
extern const u16 gFollowingMonPic_ShayminNormalPal[];
extern const u16 gFollowingMonPic_ShedinjaNormalPal[];
extern const u16 gFollowingMonPic_ShelgonNormalPal[];
extern const u16 gFollowingMonPic_ShellderNormalPal[];
extern const u16 gFollowingMonPic_ShellosNormalPal[];
extern const u16 gFollowingMonPic_ShelmetNormalPal[];
extern const u16 gFollowingMonPic_ShieldonNormalPal[];
extern const u16 gFollowingMonPic_ShiftryNormalPal[];
extern const u16 gFollowingMonPic_ShiinoticNormalPal[];
extern const u16 gFollowingMonPic_ShinxNormalPal[];
extern const u16 gFollowingMonPic_ShroodleNormalPal[];
extern const u16 gFollowingMonPic_ShroomishNormalPal[];
extern const u16 gFollowingMonPic_ShuckleNormalPal[];
extern const u16 gFollowingMonPic_ShuppetNormalPal[];
extern const u16 gFollowingMonPic_SigilyphNormalPal[];
extern const u16 gFollowingMonPic_SilcoonNormalPal[];
extern const u16 gFollowingMonPic_SilicobraNormalPal[];
extern const u16 gFollowingMonPic_SilvallyNormalPal[];
extern const u16 gFollowingMonPic_SimipourNormalPal[];
extern const u16 gFollowingMonPic_SimisageNormalPal[];
extern const u16 gFollowingMonPic_SimisearNormalPal[];
extern const u16 gFollowingMonPic_SinistchaNormalPal[];
extern const u16 gFollowingMonPic_SinisteaNormalPal[];
extern const u16 gFollowingMonPic_SirfetchdNormalPal[];
extern const u16 gFollowingMonPic_SizzlipedeNormalPal[];
extern const u16 gFollowingMonPic_SkarmoryNormalPal[];
extern const u16 gFollowingMonPic_SkeledirgeNormalPal[];
extern const u16 gFollowingMonPic_SkiddoNormalPal[];
extern const u16 gFollowingMonPic_SkiploomNormalPal[];
extern const u16 gFollowingMonPic_SkittyNormalPal[];
extern const u16 gFollowingMonPic_SkorupiNormalPal[];
extern const u16 gFollowingMonPic_SkrelpNormalPal[];
extern const u16 gFollowingMonPic_SkuntankNormalPal[];
extern const u16 gFollowingMonPic_SkwovetNormalPal[];
extern const u16 gFollowingMonPic_SlakingNormalPal[];
extern const u16 gFollowingMonPic_SlakothNormalPal[];
extern const u16 gFollowingMonPic_SliggooNormalPal[];
extern const u16 gFollowingMonPic_Slither_wingNormalPal[];
extern const u16 gFollowingMonPic_SlowbroNormalPal[];
extern const u16 gFollowingMonPic_SlowkingNormalPal[];
extern const u16 gFollowingMonPic_SlowpokeNormalPal[];
extern const u16 gFollowingMonPic_SlugmaNormalPal[];
extern const u16 gFollowingMonPic_SlurpuffNormalPal[];
extern const u16 gFollowingMonPic_SmeargleNormalPal[];
extern const u16 gFollowingMonPic_SmolivNormalPal[];
extern const u16 gFollowingMonPic_SmoochumNormalPal[];
extern const u16 gFollowingMonPic_SneaselNormalPal[];
extern const u16 gFollowingMonPic_SneaslerNormalPal[];
extern const u16 gFollowingMonPic_SnivyNormalPal[];
extern const u16 gFollowingMonPic_SnomNormalPal[];
extern const u16 gFollowingMonPic_SnorlaxNormalPal[];
extern const u16 gFollowingMonPic_SnoruntNormalPal[];
extern const u16 gFollowingMonPic_SnoverNormalPal[];
extern const u16 gFollowingMonPic_SnubbullNormalPal[];
extern const u16 gFollowingMonPic_SobbleNormalPal[];
extern const u16 gFollowingMonPic_SolgaleoNormalPal[];
extern const u16 gFollowingMonPic_SolosisNormalPal[];
extern const u16 gFollowingMonPic_SolrockNormalPal[];
extern const u16 gFollowingMonPic_SpearowNormalPal[];
extern const u16 gFollowingMonPic_SpectrierNormalPal[];
extern const u16 gFollowingMonPic_SpewpaNormalPal[];
extern const u16 gFollowingMonPic_SphealNormalPal[];
extern const u16 gFollowingMonPic_SpidopsNormalPal[];
extern const u16 gFollowingMonPic_SpinarakNormalPal[];
extern const u16 gFollowingMonPic_SpindaNormalPal[];
extern const u16 gFollowingMonPic_SpiritombNormalPal[];
extern const u16 gFollowingMonPic_SpoinkNormalPal[];
extern const u16 gFollowingMonPic_SprigatitoNormalPal[];
extern const u16 gFollowingMonPic_SpritzeeNormalPal[];
extern const u16 gFollowingMonPic_SquawkabillyNormalPal[];
extern const u16 gFollowingMonPic_SquirtleNormalPal[];
extern const u16 gFollowingMonPic_StakatakaNormalPal[];
extern const u16 gFollowingMonPic_StantlerNormalPal[];
extern const u16 gFollowingMonPic_StaraptorNormalPal[];
extern const u16 gFollowingMonPic_StaraviaNormalPal[];
extern const u16 gFollowingMonPic_StarlyNormalPal[];
extern const u16 gFollowingMonPic_StarmieNormalPal[];
extern const u16 gFollowingMonPic_StaryuNormalPal[];
extern const u16 gFollowingMonPic_SteelixNormalPal[];
extern const u16 gFollowingMonPic_SteeneeNormalPal[];
extern const u16 gFollowingMonPic_StonjournerNormalPal[];
extern const u16 gFollowingMonPic_StoutlandNormalPal[];
extern const u16 gFollowingMonPic_StuffulNormalPal[];
extern const u16 gFollowingMonPic_StunfiskNormalPal[];
extern const u16 gFollowingMonPic_StunkyNormalPal[];
extern const u16 gFollowingMonPic_SudowoodoNormalPal[];
extern const u16 gFollowingMonPic_SuicuneNormalPal[];
extern const u16 gFollowingMonPic_SunfloraNormalPal[];
extern const u16 gFollowingMonPic_SunkernNormalPal[];
extern const u16 gFollowingMonPic_SurskitNormalPal[];
extern const u16 gFollowingMonPic_SwabluNormalPal[];
extern const u16 gFollowingMonPic_SwadloonNormalPal[];
extern const u16 gFollowingMonPic_SwalotNormalPal[];
extern const u16 gFollowingMonPic_SwampertNormalPal[];
extern const u16 gFollowingMonPic_SwannaNormalPal[];
extern const u16 gFollowingMonPic_SwellowNormalPal[];
extern const u16 gFollowingMonPic_SwinubNormalPal[];
extern const u16 gFollowingMonPic_SwirlixNormalPal[];
extern const u16 gFollowingMonPic_SwoobatNormalPal[];
extern const u16 gFollowingMonPic_SylveonNormalPal[];
extern const u16 gFollowingMonPic_TadbulbNormalPal[];
extern const u16 gFollowingMonPic_TaillowNormalPal[];
extern const u16 gFollowingMonPic_TalonflameNormalPal[];
extern const u16 gFollowingMonPic_TandemausNormalPal[];
extern const u16 gFollowingMonPic_TangelaNormalPal[];
extern const u16 gFollowingMonPic_TangrowthNormalPal[];
extern const u16 gFollowingMonPic_Tapu_buluNormalPal[];
extern const u16 gFollowingMonPic_Tapu_finiNormalPal[];
extern const u16 gFollowingMonPic_Tapu_kokoNormalPal[];
extern const u16 gFollowingMonPic_Tapu_leleNormalPal[];
extern const u16 gFollowingMonPic_TarountulaNormalPal[];
extern const u16 gFollowingMonPic_TatsugiriNormalPal[];
extern const u16 gFollowingMonPic_TaurosNormalPal[];
extern const u16 gFollowingMonPic_TeddiursaNormalPal[];
extern const u16 gFollowingMonPic_TentacoolNormalPal[];
extern const u16 gFollowingMonPic_TentacruelNormalPal[];
extern const u16 gFollowingMonPic_TepigNormalPal[];
extern const u16 gFollowingMonPic_TerapagosNormalPal[];
extern const u16 gFollowingMonPic_TerrakionNormalPal[];
extern const u16 gFollowingMonPic_ThievulNormalPal[];
extern const u16 gFollowingMonPic_ThrohNormalPal[];
extern const u16 gFollowingMonPic_ThundurusNormalPal[];
extern const u16 gFollowingMonPic_ThwackeyNormalPal[];
extern const u16 gFollowingMonPic_TimburrNormalPal[];
extern const u16 gFollowingMonPic_Ting_luNormalPal[];
extern const u16 gFollowingMonPic_TinkatinkNormalPal[];
extern const u16 gFollowingMonPic_TinkatonNormalPal[];
extern const u16 gFollowingMonPic_TinkatuffNormalPal[];
extern const u16 gFollowingMonPic_TirtougaNormalPal[];
extern const u16 gFollowingMonPic_ToedscoolNormalPal[];
extern const u16 gFollowingMonPic_ToedscruelNormalPal[];
extern const u16 gFollowingMonPic_TogedemaruNormalPal[];
extern const u16 gFollowingMonPic_TogekissNormalPal[];
extern const u16 gFollowingMonPic_TogepiNormalPal[];
extern const u16 gFollowingMonPic_TogeticNormalPal[];
extern const u16 gFollowingMonPic_TorchicNormalPal[];
extern const u16 gFollowingMonPic_TorkoalNormalPal[];
extern const u16 gFollowingMonPic_TornadusNormalPal[];
extern const u16 gFollowingMonPic_TorracatNormalPal[];
extern const u16 gFollowingMonPic_TorterraNormalPal[];
extern const u16 gFollowingMonPic_TotodileNormalPal[];
extern const u16 gFollowingMonPic_ToucannonNormalPal[];
extern const u16 gFollowingMonPic_ToxapexNormalPal[];
extern const u16 gFollowingMonPic_ToxelNormalPal[];
extern const u16 gFollowingMonPic_ToxicroakNormalPal[];
extern const u16 gFollowingMonPic_ToxtricityNormalPal[];
extern const u16 gFollowingMonPic_TranquillNormalPal[];
extern const u16 gFollowingMonPic_TrapinchNormalPal[];
extern const u16 gFollowingMonPic_TreeckoNormalPal[];
extern const u16 gFollowingMonPic_TrevenantNormalPal[];
extern const u16 gFollowingMonPic_TropiusNormalPal[];
extern const u16 gFollowingMonPic_TrubbishNormalPal[];
extern const u16 gFollowingMonPic_TrumbeakNormalPal[];
extern const u16 gFollowingMonPic_TsareenaNormalPal[];
extern const u16 gFollowingMonPic_TurtonatorNormalPal[];
extern const u16 gFollowingMonPic_TurtwigNormalPal[];
extern const u16 gFollowingMonPic_TympoleNormalPal[];
extern const u16 gFollowingMonPic_TynamoNormalPal[];
extern const u16 gFollowingMonPic_Type_nullNormalPal[];
extern const u16 gFollowingMonPic_TyphlosionNormalPal[];
extern const u16 gFollowingMonPic_TyranitarNormalPal[];
extern const u16 gFollowingMonPic_TyrantrumNormalPal[];
extern const u16 gFollowingMonPic_TyrogueNormalPal[];
extern const u16 gFollowingMonPic_TyruntNormalPal[];
extern const u16 gFollowingMonPic_UmbreonNormalPal[];
extern const u16 gFollowingMonPic_UnfezantNormalPal[];
extern const u16 gFollowingMonPic_UnownNormalPal[];
extern const u16 gFollowingMonPic_UrsalunaNormalPal[];
extern const u16 gFollowingMonPic_UrsaringNormalPal[];
extern const u16 gFollowingMonPic_UrshifuNormalPal[];
extern const u16 gFollowingMonPic_UxieNormalPal[];
extern const u16 gFollowingMonPic_VanillishNormalPal[];
extern const u16 gFollowingMonPic_VanilliteNormalPal[];
extern const u16 gFollowingMonPic_VanilluxeNormalPal[];
extern const u16 gFollowingMonPic_VaporeonNormalPal[];
extern const u16 gFollowingMonPic_VaroomNormalPal[];
extern const u16 gFollowingMonPic_VeluzaNormalPal[];
extern const u16 gFollowingMonPic_VenipedeNormalPal[];
extern const u16 gFollowingMonPic_VenomothNormalPal[];
extern const u16 gFollowingMonPic_VenonatNormalPal[];
extern const u16 gFollowingMonPic_VenusaurNormalPal[];
extern const u16 gFollowingMonPic_VespiquenNormalPal[];
extern const u16 gFollowingMonPic_VibravaNormalPal[];
extern const u16 gFollowingMonPic_VictiniNormalPal[];
extern const u16 gFollowingMonPic_VictreebelNormalPal[];
extern const u16 gFollowingMonPic_VigorothNormalPal[];
extern const u16 gFollowingMonPic_VikavoltNormalPal[];
extern const u16 gFollowingMonPic_VileplumeNormalPal[];
extern const u16 gFollowingMonPic_VirizionNormalPal[];
extern const u16 gFollowingMonPic_VivillonNormalPal[];
extern const u16 gFollowingMonPic_VolbeatNormalPal[];
extern const u16 gFollowingMonPic_VolcanionNormalPal[];
extern const u16 gFollowingMonPic_VolcaronaNormalPal[];
extern const u16 gFollowingMonPic_VoltorbNormalPal[];
extern const u16 gFollowingMonPic_VullabyNormalPal[];
extern const u16 gFollowingMonPic_VulpixNormalPal[];
extern const u16 gFollowingMonPic_WailmerNormalPal[];
extern const u16 gFollowingMonPic_WailordNormalPal[];
extern const u16 gFollowingMonPic_Walking_wakeNormalPal[];
extern const u16 gFollowingMonPic_WalreinNormalPal[];
extern const u16 gFollowingMonPic_WartortleNormalPal[];
extern const u16 gFollowingMonPic_WatchogNormalPal[];
extern const u16 gFollowingMonPic_WattrelNormalPal[];
extern const u16 gFollowingMonPic_WeavileNormalPal[];
extern const u16 gFollowingMonPic_WeedleNormalPal[];
extern const u16 gFollowingMonPic_WeepinbellNormalPal[];
extern const u16 gFollowingMonPic_WeezingNormalPal[];
extern const u16 gFollowingMonPic_WhimsicottNormalPal[];
extern const u16 gFollowingMonPic_WhirlipedeNormalPal[];
extern const u16 gFollowingMonPic_WhiscashNormalPal[];
extern const u16 gFollowingMonPic_WhismurNormalPal[];
extern const u16 gFollowingMonPic_WigglytuffNormalPal[];
extern const u16 gFollowingMonPic_WiglettNormalPal[];
extern const u16 gFollowingMonPic_WimpodNormalPal[];
extern const u16 gFollowingMonPic_WingullNormalPal[];
extern const u16 gFollowingMonPic_WishiwashiNormalPal[];
extern const u16 gFollowingMonPic_WobbuffetNormalPal[];
extern const u16 gFollowingMonPic_WoobatNormalPal[];
extern const u16 gFollowingMonPic_WoolooNormalPal[];
extern const u16 gFollowingMonPic_WooperNormalPal[];
extern const u16 gFollowingMonPic_WormadamNormalPal[];
extern const u16 gFollowingMonPic_Wo_chienNormalPal[];
extern const u16 gFollowingMonPic_WugtrioNormalPal[];
extern const u16 gFollowingMonPic_WurmpleNormalPal[];
extern const u16 gFollowingMonPic_WynautNormalPal[];
extern const u16 gFollowingMonPic_WyrdeerNormalPal[];
extern const u16 gFollowingMonPic_XatuNormalPal[];
extern const u16 gFollowingMonPic_XerneasNormalPal[];
extern const u16 gFollowingMonPic_XurkitreeNormalPal[];
extern const u16 gFollowingMonPic_YamaskNormalPal[];
extern const u16 gFollowingMonPic_YamperNormalPal[];
extern const u16 gFollowingMonPic_YanmaNormalPal[];
extern const u16 gFollowingMonPic_YanmegaNormalPal[];
extern const u16 gFollowingMonPic_YungoosNormalPal[];
extern const u16 gFollowingMonPic_YveltalNormalPal[];
extern const u16 gFollowingMonPic_ZacianNormalPal[];
extern const u16 gFollowingMonPic_ZamazentaNormalPal[];
extern const u16 gFollowingMonPic_ZangooseNormalPal[];
extern const u16 gFollowingMonPic_ZapdosNormalPal[];
extern const u16 gFollowingMonPic_ZarudeNormalPal[];
extern const u16 gFollowingMonPic_ZebstrikaNormalPal[];
extern const u16 gFollowingMonPic_ZekromNormalPal[];
extern const u16 gFollowingMonPic_ZeraoraNormalPal[];
extern const u16 gFollowingMonPic_ZigzagoonNormalPal[];
extern const u16 gFollowingMonPic_ZoroarkNormalPal[];
extern const u16 gFollowingMonPic_ZoruaNormalPal[];
extern const u16 gFollowingMonPic_ZubatNormalPal[];
extern const u16 gFollowingMonPic_ZweilousNormalPal[];
extern const u16 gFollowingMonPic_ZygardeNormalPal[];
extern const u16 gFollowingMonPic_PonytaGalarianNormalPal[];
extern const u16 gFollowingMonPic_QwilfishHisuianNormalPal[];
extern const u16 gFollowingMonPic_RaichuAlolanNormalPal[];
extern const u16 gFollowingMonPic_RapidashGalarianNormalPal[];
extern const u16 gFollowingMonPic_RaticateAlolanNormalPal[];
extern const u16 gFollowingMonPic_RattataAlolanNormalPal[];
extern const u16 gFollowingMonPic_RotomFanNormalPal[];
extern const u16 gFollowingMonPic_RotomFrostNormalPal[];
extern const u16 gFollowingMonPic_RotomHeatNormalPal[];
extern const u16 gFollowingMonPic_RotomMowNormalPal[];
extern const u16 gFollowingMonPic_RotomWashNormalPal[];
extern const u16 gFollowingMonPic_SamurottHisuianNormalPal[];
extern const u16 gFollowingMonPic_SandshrewAlolanNormalPal[];
extern const u16 gFollowingMonPic_SandslashAlolanNormalPal[];
extern const u16 gFollowingMonPic_SawsbuckAutumnNormalPal[];
extern const u16 gFollowingMonPic_SawsbuckSummerNormalPal[];
extern const u16 gFollowingMonPic_SawsbuckWinterNormalPal[];
extern const u16 gFollowingMonPic_ShayminSkyNormalPal[];
extern const u16 gFollowingMonPic_ShellosEast_seaNormalPal[];
extern const u16 gFollowingMonPic_SliggooHisuianNormalPal[];
extern const u16 gFollowingMonPic_SlowbroGalarianNormalPal[];
extern const u16 gFollowingMonPic_SlowkingGalarianNormalPal[];
extern const u16 gFollowingMonPic_SlowpokeGalarianNormalPal[];
extern const u16 gFollowingMonPic_SneaselHisuianNormalPal[];
extern const u16 gFollowingMonPic_SquawkabillyBlue_plumageNormalPal[];
extern const u16 gFollowingMonPic_SquawkabillyWhite_plumageNormalPal[];
extern const u16 gFollowingMonPic_SquawkabillyYellow_plumageNormalPal[];
extern const u16 gFollowingMonPic_StunfiskGalarianNormalPal[];
extern const u16 gFollowingMonPic_TatsugiriDroopyNormalPal[];
extern const u16 gFollowingMonPic_TatsugiriStretchyNormalPal[];
extern const u16 gFollowingMonPic_TaurosPaldean_aqua_breedNormalPal[];
extern const u16 gFollowingMonPic_TaurosPaldean_blaze_breedNormalPal[];
extern const u16 gFollowingMonPic_TaurosPaldean_combat_breedNormalPal[];
extern const u16 gFollowingMonPic_TerapagosTerastalNormalPal[];
extern const u16 gFollowingMonPic_ThundurusTherianNormalPal[];
extern const u16 gFollowingMonPic_TornadusTherianNormalPal[];
extern const u16 gFollowingMonPic_ToxtricityLow_keyNormalPal[];
extern const u16 gFollowingMonPic_TyphlosionHisuianNormalPal[];
extern const u16 gFollowingMonPic_UrsalunaBloodmoonNormalPal[];
extern const u16 gFollowingMonPic_VivillonArchipelagoNormalPal[];
extern const u16 gFollowingMonPic_VivillonContinentalNormalPal[];
extern const u16 gFollowingMonPic_VivillonElegantNormalPal[];
extern const u16 gFollowingMonPic_VivillonFancyNormalPal[];
extern const u16 gFollowingMonPic_VivillonGardenNormalPal[];
extern const u16 gFollowingMonPic_VivillonHigh_plainsNormalPal[];
extern const u16 gFollowingMonPic_VivillonJungleNormalPal[];
extern const u16 gFollowingMonPic_VivillonMarineNormalPal[];
extern const u16 gFollowingMonPic_VivillonMeadowNormalPal[];
extern const u16 gFollowingMonPic_VivillonModernNormalPal[];
extern const u16 gFollowingMonPic_VivillonMonsoonNormalPal[];
extern const u16 gFollowingMonPic_VivillonOceanNormalPal[];
extern const u16 gFollowingMonPic_VivillonPoke_ballNormalPal[];
extern const u16 gFollowingMonPic_VivillonPolarNormalPal[];
extern const u16 gFollowingMonPic_VivillonRiverNormalPal[];
extern const u16 gFollowingMonPic_VivillonSandstormNormalPal[];
extern const u16 gFollowingMonPic_VivillonSavannaNormalPal[];
extern const u16 gFollowingMonPic_VivillonSunNormalPal[];
extern const u16 gFollowingMonPic_VivillonTundraNormalPal[];
extern const u16 gFollowingMonPic_VoltorbHisuianNormalPal[];
extern const u16 gFollowingMonPic_VulpixAlolanNormalPal[];
extern const u16 gFollowingMonPic_WeezingGalarianNormalPal[];
extern const u16 gFollowingMonPic_WooperWooper_paldeanNormalPal[];
extern const u16 gFollowingMonPic_WormadamSandy_cloakNormalPal[];
extern const u16 gFollowingMonPic_WormadamTrash_cloakNormalPal[];
extern const u16 gFollowingMonPic_XerneasActiveNormalPal[];
extern const u16 gFollowingMonPic_YamaskGalarianNormalPal[];
extern const u16 gFollowingMonPic_ZacianCrowned_swordNormalPal[];
extern const u16 gFollowingMonPic_ZamazentaCrowned_shieldNormalPal[];
extern const u16 gFollowingMonPic_ZapdosGalarianNormalPal[];
extern const u16 gFollowingMonPic_ZigzagoonGalarianNormalPal[];
extern const u16 gFollowingMonPic_ZoroarkHisuianNormalPal[];
extern const u16 gFollowingMonPic_ZoruaHisuianNormalPal[];
extern const u16 gFollowingMonPic_Zygarde10_percentNormalPal[];
extern const u16 gFollowingMonPic_ZygardeCompleteNormalPal[];

//Shiny Pals
extern const u16 gFollowingMonPic_AlcremieStrawberryShinyPal[];
extern const u16 gFollowingMonPic_ArcanineHisuianShinyPal[];
extern const u16 gFollowingMonPic_ArceusBugShinyPal[];
extern const u16 gFollowingMonPic_ArceusDarkShinyPal[];
extern const u16 gFollowingMonPic_ArceusDragonShinyPal[];
extern const u16 gFollowingMonPic_ArceusElectricShinyPal[];
extern const u16 gFollowingMonPic_ArceusFairyShinyPal[];
extern const u16 gFollowingMonPic_ArceusFightingShinyPal[];
extern const u16 gFollowingMonPic_ArceusFireShinyPal[];
extern const u16 gFollowingMonPic_ArceusFlyingShinyPal[];
extern const u16 gFollowingMonPic_ArceusGhostShinyPal[];
extern const u16 gFollowingMonPic_ArceusGrassShinyPal[];
extern const u16 gFollowingMonPic_ArceusGroundShinyPal[];
extern const u16 gFollowingMonPic_ArceusIceShinyPal[];
extern const u16 gFollowingMonPic_ArceusPoisonShinyPal[];
extern const u16 gFollowingMonPic_ArceusPsychicShinyPal[];
extern const u16 gFollowingMonPic_ArceusRockShinyPal[];
extern const u16 gFollowingMonPic_ArceusSteelShinyPal[];
extern const u16 gFollowingMonPic_ArceusWaterShinyPal[];
extern const u16 gFollowingMonPic_ArticunoGalarianShinyPal[];
extern const u16 gFollowingMonPic_AvaluggHisuianShinyPal[];
extern const u16 gFollowingMonPic_BasculegionFemaleShinyPal[];
extern const u16 gFollowingMonPic_BasculinBlue_stripedShinyPal[];
extern const u16 gFollowingMonPic_BasculinWhite_stripedShinyPal[];
extern const u16 gFollowingMonPic_BraviaryHisuianShinyPal[];
extern const u16 gFollowingMonPic_BurmySandy_cloakShinyPal[];
extern const u16 gFollowingMonPic_BurmyTrash_cloakShinyPal[];
extern const u16 gFollowingMonPic_CalyrexIce_riderShinyPal[];
extern const u16 gFollowingMonPic_CalyrexShadow_riderShinyPal[];
extern const u16 gFollowingMonPic_CastformRainyShinyPal[];
extern const u16 gFollowingMonPic_CastformSnowyShinyPal[];
extern const u16 gFollowingMonPic_CastformSunnyShinyPal[];
extern const u16 gFollowingMonPic_CorsolaGalarianShinyPal[];
extern const u16 gFollowingMonPic_DarmanitanGalarianShinyPal[];
extern const u16 gFollowingMonPic_DarumakaGalarianShinyPal[];
extern const u16 gFollowingMonPic_DecidueyeHisuianShinyPal[];
extern const u16 gFollowingMonPic_DeerlingAutumnShinyPal[];
extern const u16 gFollowingMonPic_DeerlingSummerShinyPal[];
extern const u16 gFollowingMonPic_DeerlingWinterShinyPal[];
extern const u16 gFollowingMonPic_DeoxysAttackShinyPal[];
extern const u16 gFollowingMonPic_DeoxysDefenseShinyPal[];
extern const u16 gFollowingMonPic_DeoxysSpeedShinyPal[];
extern const u16 gFollowingMonPic_DialgaOriginShinyPal[];
extern const u16 gFollowingMonPic_DiglettAlolanShinyPal[];
extern const u16 gFollowingMonPic_DudunsparceThree_segmentShinyPal[];
extern const u16 gFollowingMonPic_DugtrioAlolanShinyPal[];
extern const u16 gFollowingMonPic_ElectrodeHisuianShinyPal[];
extern const u16 gFollowingMonPic_EnamorusTherianShinyPal[];
extern const u16 gFollowingMonPic_ExeggutorAlolanShinyPal[];
extern const u16 gFollowingMonPic_FarfetchdGalarianShinyPal[];
extern const u16 gFollowingMonPic_FlabebeBlue_flowerShinyPal[];
extern const u16 gFollowingMonPic_FlabebeOrange_flowerShinyPal[];
extern const u16 gFollowingMonPic_FlabebeWhite_flowerShinyPal[];
extern const u16 gFollowingMonPic_FlabebeYellow_flowerShinyPal[];
extern const u16 gFollowingMonPic_FloetteBlue_flowerShinyPal[];
extern const u16 gFollowingMonPic_FloetteEternal_flowerShinyPal[];
extern const u16 gFollowingMonPic_FloetteOrange_flowerShinyPal[];
extern const u16 gFollowingMonPic_FloetteWhite_flowerShinyPal[];
extern const u16 gFollowingMonPic_FloetteYellow_flowerShinyPal[];
extern const u16 gFollowingMonPic_FlorgesBlue_flowerShinyPal[];
extern const u16 gFollowingMonPic_FlorgesOrange_flowerShinyPal[];
extern const u16 gFollowingMonPic_FlorgesWhite_flowerShinyPal[];
extern const u16 gFollowingMonPic_FlorgesYellow_flowerShinyPal[];
extern const u16 gFollowingMonPic_FurfrouDandy_trimShinyPal[];
extern const u16 gFollowingMonPic_FurfrouDebutante_trimShinyPal[];
extern const u16 gFollowingMonPic_FurfrouDiamond_trimShinyPal[];
extern const u16 gFollowingMonPic_FurfrouHeart_trimShinyPal[];
extern const u16 gFollowingMonPic_FurfrouKabuki_trimShinyPal[];
extern const u16 gFollowingMonPic_FurfrouLa_reine_trimShinyPal[];
extern const u16 gFollowingMonPic_FurfrouMatron_trimShinyPal[];
extern const u16 gFollowingMonPic_FurfrouPharaoh_trimShinyPal[];
extern const u16 gFollowingMonPic_FurfrouStar_trimShinyPal[];
extern const u16 gFollowingMonPic_GastrodonEast_seaShinyPal[];
extern const u16 gFollowingMonPic_GeodudeAlolanShinyPal[];
extern const u16 gFollowingMonPic_GiratinaOriginShinyPal[];
extern const u16 gFollowingMonPic_GolemAlolanShinyPal[];
extern const u16 gFollowingMonPic_GoodraHisuianShinyPal[];
extern const u16 gFollowingMonPic_GravelerAlolanShinyPal[];
extern const u16 gFollowingMonPic_GrimerAlolanShinyPal[];
extern const u16 gFollowingMonPic_GrowlitheHisuianShinyPal[];
extern const u16 gFollowingMonPic_HoopaUnboundShinyPal[];
extern const u16 gFollowingMonPic_IndeedeeFemaleShinyPal[];
extern const u16 gFollowingMonPic_KyuremBlackShinyPal[];
extern const u16 gFollowingMonPic_KyuremWhiteShinyPal[];
extern const u16 gFollowingMonPic_LandorusTherianShinyPal[];
extern const u16 gFollowingMonPic_LilligantHisuianShinyPal[];
extern const u16 gFollowingMonPic_LinooneGalarianShinyPal[];
extern const u16 gFollowingMonPic_LycanrocDuskShinyPal[];
extern const u16 gFollowingMonPic_LycanrocMidnightShinyPal[];
extern const u16 gFollowingMonPic_MagearnaOriginal_colorShinyPal[];
extern const u16 gFollowingMonPic_MarowakAlolanShinyPal[];
extern const u16 gFollowingMonPic_MausholdFourShinyPal[];
extern const u16 gFollowingMonPic_MeloettaPirouetteShinyPal[];
extern const u16 gFollowingMonPic_MeowsticFemaleShinyPal[];
extern const u16 gFollowingMonPic_MeowthAlolanShinyPal[];
extern const u16 gFollowingMonPic_MeowthGalarianShinyPal[];
extern const u16 gFollowingMonPic_MoltresGalarianShinyPal[];
extern const u16 gFollowingMonPic_Mr_mimeGalarianShinyPal[];
extern const u16 gFollowingMonPic_MukAlolanShinyPal[];
extern const u16 gFollowingMonPic_NecrozmaDawn_wingsShinyPal[];
extern const u16 gFollowingMonPic_NecrozmaDusk_maneShinyPal[];
extern const u16 gFollowingMonPic_NinetalesAlolanShinyPal[];
extern const u16 gFollowingMonPic_OgerponCornerstoneShinyPal[];
extern const u16 gFollowingMonPic_OgerponHearthflameShinyPal[];
extern const u16 gFollowingMonPic_OgerponWellspringShinyPal[];
extern const u16 gFollowingMonPic_OinkologneFemaleShinyPal[];
extern const u16 gFollowingMonPic_OricorioPauShinyPal[];
extern const u16 gFollowingMonPic_OricorioPom_pomShinyPal[];
extern const u16 gFollowingMonPic_OricorioSensuShinyPal[];
extern const u16 gFollowingMonPic_PalafinHeroShinyPal[];
extern const u16 gFollowingMonPic_PalkiaOriginShinyPal[];
extern const u16 gFollowingMonPic_PersianAlolanShinyPal[];
extern const u16 gFollowingMonPic_PichuSpiky_earedShinyPal[];
extern const u16 gFollowingMonPic_AbomasnowShinyPal[];
extern const u16 gFollowingMonPic_AbraShinyPal[];
extern const u16 gFollowingMonPic_AbsolShinyPal[];
extern const u16 gFollowingMonPic_AccelgorShinyPal[];
extern const u16 gFollowingMonPic_AegislashShinyPal[];
extern const u16 gFollowingMonPic_AerodactylShinyPal[];
extern const u16 gFollowingMonPic_AggronShinyPal[];
extern const u16 gFollowingMonPic_AipomShinyPal[];
extern const u16 gFollowingMonPic_AlakazamShinyPal[];
extern const u16 gFollowingMonPic_AlomomolaShinyPal[];
extern const u16 gFollowingMonPic_AltariaShinyPal[];
extern const u16 gFollowingMonPic_AmauraShinyPal[];
extern const u16 gFollowingMonPic_AmbipomShinyPal[];
extern const u16 gFollowingMonPic_AmoongussShinyPal[];
extern const u16 gFollowingMonPic_AmpharosShinyPal[];
extern const u16 gFollowingMonPic_AnnihilapeShinyPal[];
extern const u16 gFollowingMonPic_AnorithShinyPal[];
extern const u16 gFollowingMonPic_AppletunShinyPal[];
extern const u16 gFollowingMonPic_ApplinShinyPal[];
extern const u16 gFollowingMonPic_AraquanidShinyPal[];
extern const u16 gFollowingMonPic_ArbokShinyPal[];
extern const u16 gFollowingMonPic_ArbolivaShinyPal[];
extern const u16 gFollowingMonPic_ArcanineShinyPal[];
extern const u16 gFollowingMonPic_ArceusShinyPal[];
extern const u16 gFollowingMonPic_ArchaludonShinyPal[];
extern const u16 gFollowingMonPic_ArchenShinyPal[];
extern const u16 gFollowingMonPic_ArcheopsShinyPal[];
extern const u16 gFollowingMonPic_ArctibaxShinyPal[];
extern const u16 gFollowingMonPic_ArctovishShinyPal[];
extern const u16 gFollowingMonPic_ArctozoltShinyPal[];
extern const u16 gFollowingMonPic_AriadosShinyPal[];
extern const u16 gFollowingMonPic_ArmaldoShinyPal[];
extern const u16 gFollowingMonPic_ArmarougeShinyPal[];
extern const u16 gFollowingMonPic_AromatisseShinyPal[];
extern const u16 gFollowingMonPic_AronShinyPal[];
extern const u16 gFollowingMonPic_ArrokudaShinyPal[];
extern const u16 gFollowingMonPic_ArticunoShinyPal[];
extern const u16 gFollowingMonPic_AudinoShinyPal[];
extern const u16 gFollowingMonPic_AurorusShinyPal[];
extern const u16 gFollowingMonPic_AvaluggShinyPal[];
extern const u16 gFollowingMonPic_AxewShinyPal[];
extern const u16 gFollowingMonPic_AzelfShinyPal[];
extern const u16 gFollowingMonPic_AzumarillShinyPal[];
extern const u16 gFollowingMonPic_AzurillShinyPal[];
extern const u16 gFollowingMonPic_BagonShinyPal[];
extern const u16 gFollowingMonPic_BaltoyShinyPal[];
extern const u16 gFollowingMonPic_BanetteShinyPal[];
extern const u16 gFollowingMonPic_BarbaracleShinyPal[];
extern const u16 gFollowingMonPic_BarboachShinyPal[];
extern const u16 gFollowingMonPic_BarraskewdaShinyPal[];
extern const u16 gFollowingMonPic_BasculegionShinyPal[];
extern const u16 gFollowingMonPic_BasculinShinyPal[];
extern const u16 gFollowingMonPic_BastiodonShinyPal[];
extern const u16 gFollowingMonPic_BaxcaliburShinyPal[];
extern const u16 gFollowingMonPic_BayleefShinyPal[];
extern const u16 gFollowingMonPic_BearticShinyPal[];
extern const u16 gFollowingMonPic_BeautiflyShinyPal[];
extern const u16 gFollowingMonPic_BeedrillShinyPal[];
extern const u16 gFollowingMonPic_BeheeyemShinyPal[];
extern const u16 gFollowingMonPic_BeldumShinyPal[];
extern const u16 gFollowingMonPic_BelliboltShinyPal[];
extern const u16 gFollowingMonPic_BellossomShinyPal[];
extern const u16 gFollowingMonPic_BellsproutShinyPal[];
extern const u16 gFollowingMonPic_BergmiteShinyPal[];
extern const u16 gFollowingMonPic_BewearShinyPal[];
extern const u16 gFollowingMonPic_BibarelShinyPal[];
extern const u16 gFollowingMonPic_BidoofShinyPal[];
extern const u16 gFollowingMonPic_BinacleShinyPal[];
extern const u16 gFollowingMonPic_BisharpShinyPal[];
extern const u16 gFollowingMonPic_BlacephalonShinyPal[];
extern const u16 gFollowingMonPic_BlastoiseShinyPal[];
extern const u16 gFollowingMonPic_BlazikenShinyPal[];
extern const u16 gFollowingMonPic_BlipbugShinyPal[];
extern const u16 gFollowingMonPic_BlisseyShinyPal[];
extern const u16 gFollowingMonPic_BlitzleShinyPal[];
extern const u16 gFollowingMonPic_BoldoreShinyPal[];
extern const u16 gFollowingMonPic_BoltundShinyPal[];
extern const u16 gFollowingMonPic_BombirdierShinyPal[];
extern const u16 gFollowingMonPic_BonslyShinyPal[];
extern const u16 gFollowingMonPic_BouffalantShinyPal[];
extern const u16 gFollowingMonPic_BounsweetShinyPal[];
extern const u16 gFollowingMonPic_BraixenShinyPal[];
extern const u16 gFollowingMonPic_BrambleghastShinyPal[];
extern const u16 gFollowingMonPic_BramblinShinyPal[];
extern const u16 gFollowingMonPic_BraviaryShinyPal[];
extern const u16 gFollowingMonPic_BreloomShinyPal[];
extern const u16 gFollowingMonPic_BrionneShinyPal[];
extern const u16 gFollowingMonPic_BronzongShinyPal[];
extern const u16 gFollowingMonPic_BronzorShinyPal[];
extern const u16 gFollowingMonPic_Brute_bonnetShinyPal[];
extern const u16 gFollowingMonPic_BruxishShinyPal[];
extern const u16 gFollowingMonPic_BudewShinyPal[];
extern const u16 gFollowingMonPic_BuizelShinyPal[];
extern const u16 gFollowingMonPic_BulbasaurShinyPal[];
extern const u16 gFollowingMonPic_BunearyShinyPal[];
extern const u16 gFollowingMonPic_BunnelbyShinyPal[];
extern const u16 gFollowingMonPic_BurmyShinyPal[];
extern const u16 gFollowingMonPic_ButterfreeShinyPal[];
extern const u16 gFollowingMonPic_BuzzwoleShinyPal[];
extern const u16 gFollowingMonPic_CacneaShinyPal[];
extern const u16 gFollowingMonPic_CacturneShinyPal[];
extern const u16 gFollowingMonPic_CalyrexShinyPal[];
extern const u16 gFollowingMonPic_CameruptShinyPal[];
extern const u16 gFollowingMonPic_CapsakidShinyPal[];
extern const u16 gFollowingMonPic_CarbinkShinyPal[];
extern const u16 gFollowingMonPic_CarkolShinyPal[];
extern const u16 gFollowingMonPic_CarnivineShinyPal[];
extern const u16 gFollowingMonPic_CarracostaShinyPal[];
extern const u16 gFollowingMonPic_CarvanhaShinyPal[];
extern const u16 gFollowingMonPic_CascoonShinyPal[];
extern const u16 gFollowingMonPic_CastformShinyPal[];
extern const u16 gFollowingMonPic_CaterpieShinyPal[];
extern const u16 gFollowingMonPic_CelebiShinyPal[];
extern const u16 gFollowingMonPic_CelesteelaShinyPal[];
extern const u16 gFollowingMonPic_CentiskorchShinyPal[];
extern const u16 gFollowingMonPic_CeruledgeShinyPal[];
extern const u16 gFollowingMonPic_CetitanShinyPal[];
extern const u16 gFollowingMonPic_CetoddleShinyPal[];
extern const u16 gFollowingMonPic_ChandelureShinyPal[];
extern const u16 gFollowingMonPic_ChanseyShinyPal[];
extern const u16 gFollowingMonPic_CharcadetShinyPal[];
extern const u16 gFollowingMonPic_CharizardShinyPal[];
extern const u16 gFollowingMonPic_CharjabugShinyPal[];
extern const u16 gFollowingMonPic_CharmanderShinyPal[];
extern const u16 gFollowingMonPic_CharmeleonShinyPal[];
extern const u16 gFollowingMonPic_ChatotShinyPal[];
extern const u16 gFollowingMonPic_CherrimShinyPal[];
extern const u16 gFollowingMonPic_CherubiShinyPal[];
extern const u16 gFollowingMonPic_ChesnaughtShinyPal[];
extern const u16 gFollowingMonPic_ChespinShinyPal[];
extern const u16 gFollowingMonPic_ChewtleShinyPal[];
extern const u16 gFollowingMonPic_Chien_paoShinyPal[];
extern const u16 gFollowingMonPic_ChikoritaShinyPal[];
extern const u16 gFollowingMonPic_ChimcharShinyPal[];
extern const u16 gFollowingMonPic_ChimechoShinyPal[];
extern const u16 gFollowingMonPic_ChinchouShinyPal[];
extern const u16 gFollowingMonPic_ChinglingShinyPal[];
extern const u16 gFollowingMonPic_Chi_yuShinyPal[];
extern const u16 gFollowingMonPic_CinccinoShinyPal[];
extern const u16 gFollowingMonPic_CinderaceShinyPal[];
extern const u16 gFollowingMonPic_ClamperlShinyPal[];
extern const u16 gFollowingMonPic_ClauncherShinyPal[];
extern const u16 gFollowingMonPic_ClawitzerShinyPal[];
extern const u16 gFollowingMonPic_ClaydolShinyPal[];
extern const u16 gFollowingMonPic_ClefableShinyPal[];
extern const u16 gFollowingMonPic_ClefairyShinyPal[];
extern const u16 gFollowingMonPic_CleffaShinyPal[];
extern const u16 gFollowingMonPic_ClobbopusShinyPal[];
extern const u16 gFollowingMonPic_ClodsireShinyPal[];
extern const u16 gFollowingMonPic_CloysterShinyPal[];
extern const u16 gFollowingMonPic_CoalossalShinyPal[];
extern const u16 gFollowingMonPic_CobalionShinyPal[];
extern const u16 gFollowingMonPic_CofagrigusShinyPal[];
extern const u16 gFollowingMonPic_CombeeShinyPal[];
extern const u16 gFollowingMonPic_CombuskenShinyPal[];
extern const u16 gFollowingMonPic_ComfeyShinyPal[];
extern const u16 gFollowingMonPic_ConkeldurrShinyPal[];
extern const u16 gFollowingMonPic_CopperajahShinyPal[];
extern const u16 gFollowingMonPic_CorphishShinyPal[];
extern const u16 gFollowingMonPic_CorsolaShinyPal[];
extern const u16 gFollowingMonPic_CorviknightShinyPal[];
extern const u16 gFollowingMonPic_CorvisquireShinyPal[];
extern const u16 gFollowingMonPic_CosmoemShinyPal[];
extern const u16 gFollowingMonPic_CosmogShinyPal[];
extern const u16 gFollowingMonPic_CottoneeShinyPal[];
extern const u16 gFollowingMonPic_CrabominableShinyPal[];
extern const u16 gFollowingMonPic_CrabrawlerShinyPal[];
extern const u16 gFollowingMonPic_CradilyShinyPal[];
extern const u16 gFollowingMonPic_CramorantShinyPal[];
extern const u16 gFollowingMonPic_CranidosShinyPal[];
extern const u16 gFollowingMonPic_CrawdauntShinyPal[];
extern const u16 gFollowingMonPic_CresseliaShinyPal[];
extern const u16 gFollowingMonPic_CroagunkShinyPal[];
extern const u16 gFollowingMonPic_CrobatShinyPal[];
extern const u16 gFollowingMonPic_CrocalorShinyPal[];
extern const u16 gFollowingMonPic_CroconawShinyPal[];
extern const u16 gFollowingMonPic_CrustleShinyPal[];
extern const u16 gFollowingMonPic_CryogonalShinyPal[];
extern const u16 gFollowingMonPic_CubchooShinyPal[];
extern const u16 gFollowingMonPic_CuboneShinyPal[];
extern const u16 gFollowingMonPic_CufantShinyPal[];
extern const u16 gFollowingMonPic_CursolaShinyPal[];
extern const u16 gFollowingMonPic_CutieflyShinyPal[];
extern const u16 gFollowingMonPic_CyclizarShinyPal[];
extern const u16 gFollowingMonPic_CyndaquilShinyPal[];
extern const u16 gFollowingMonPic_DachsbunShinyPal[];
extern const u16 gFollowingMonPic_DarkraiShinyPal[];
extern const u16 gFollowingMonPic_DarmanitanShinyPal[];
extern const u16 gFollowingMonPic_DartrixShinyPal[];
extern const u16 gFollowingMonPic_DarumakaShinyPal[];
extern const u16 gFollowingMonPic_DecidueyeShinyPal[];
extern const u16 gFollowingMonPic_DedenneShinyPal[];
extern const u16 gFollowingMonPic_DeerlingShinyPal[];
extern const u16 gFollowingMonPic_DeinoShinyPal[];
extern const u16 gFollowingMonPic_DelcattyShinyPal[];
extern const u16 gFollowingMonPic_DelibirdShinyPal[];
extern const u16 gFollowingMonPic_DelphoxShinyPal[];
extern const u16 gFollowingMonPic_DeoxysShinyPal[];
extern const u16 gFollowingMonPic_DewgongShinyPal[];
extern const u16 gFollowingMonPic_DewottShinyPal[];
extern const u16 gFollowingMonPic_DewpiderShinyPal[];
extern const u16 gFollowingMonPic_DhelmiseShinyPal[];
extern const u16 gFollowingMonPic_DialgaShinyPal[];
extern const u16 gFollowingMonPic_DiancieShinyPal[];
extern const u16 gFollowingMonPic_DiggersbyShinyPal[];
extern const u16 gFollowingMonPic_DiglettShinyPal[];
extern const u16 gFollowingMonPic_DipplinShinyPal[];
extern const u16 gFollowingMonPic_DittoShinyPal[];
extern const u16 gFollowingMonPic_DodrioShinyPal[];
extern const u16 gFollowingMonPic_DoduoShinyPal[];
extern const u16 gFollowingMonPic_DollivShinyPal[];
extern const u16 gFollowingMonPic_DondozoShinyPal[];
extern const u16 gFollowingMonPic_DonphanShinyPal[];
extern const u16 gFollowingMonPic_DottlerShinyPal[];
extern const u16 gFollowingMonPic_DoubladeShinyPal[];
extern const u16 gFollowingMonPic_DracovishShinyPal[];
extern const u16 gFollowingMonPic_DracozoltShinyPal[];
extern const u16 gFollowingMonPic_DragalgeShinyPal[];
extern const u16 gFollowingMonPic_DragapultShinyPal[];
extern const u16 gFollowingMonPic_DragonairShinyPal[];
extern const u16 gFollowingMonPic_DragoniteShinyPal[];
extern const u16 gFollowingMonPic_DrakloakShinyPal[];
extern const u16 gFollowingMonPic_DrampaShinyPal[];
extern const u16 gFollowingMonPic_DrapionShinyPal[];
extern const u16 gFollowingMonPic_DratiniShinyPal[];
extern const u16 gFollowingMonPic_DrednawShinyPal[];
extern const u16 gFollowingMonPic_DreepyShinyPal[];
extern const u16 gFollowingMonPic_DrifblimShinyPal[];
extern const u16 gFollowingMonPic_DrifloonShinyPal[];
extern const u16 gFollowingMonPic_DrilburShinyPal[];
extern const u16 gFollowingMonPic_DrizzileShinyPal[];
extern const u16 gFollowingMonPic_DrowzeeShinyPal[];
extern const u16 gFollowingMonPic_DruddigonShinyPal[];
extern const u16 gFollowingMonPic_DubwoolShinyPal[];
extern const u16 gFollowingMonPic_DucklettShinyPal[];
extern const u16 gFollowingMonPic_DudunsparceShinyPal[];
extern const u16 gFollowingMonPic_DugtrioShinyPal[];
extern const u16 gFollowingMonPic_DunsparceShinyPal[];
extern const u16 gFollowingMonPic_DuosionShinyPal[];
extern const u16 gFollowingMonPic_DuraludonShinyPal[];
extern const u16 gFollowingMonPic_DurantShinyPal[];
extern const u16 gFollowingMonPic_DusclopsShinyPal[];
extern const u16 gFollowingMonPic_DusknoirShinyPal[];
extern const u16 gFollowingMonPic_DuskullShinyPal[];
extern const u16 gFollowingMonPic_DustoxShinyPal[];
extern const u16 gFollowingMonPic_DwebbleShinyPal[];
extern const u16 gFollowingMonPic_EelektrikShinyPal[];
extern const u16 gFollowingMonPic_EelektrossShinyPal[];
extern const u16 gFollowingMonPic_EeveeShinyPal[];
extern const u16 gFollowingMonPic_EiscueShinyPal[];
extern const u16 gFollowingMonPic_EkansShinyPal[];
extern const u16 gFollowingMonPic_EldegossShinyPal[];
extern const u16 gFollowingMonPic_ElectabuzzShinyPal[];
extern const u16 gFollowingMonPic_ElectivireShinyPal[];
extern const u16 gFollowingMonPic_ElectrikeShinyPal[];
extern const u16 gFollowingMonPic_ElectrodeShinyPal[];
extern const u16 gFollowingMonPic_ElekidShinyPal[];
extern const u16 gFollowingMonPic_ElgyemShinyPal[];
extern const u16 gFollowingMonPic_EmboarShinyPal[];
extern const u16 gFollowingMonPic_EmolgaShinyPal[];
extern const u16 gFollowingMonPic_EmpoleonShinyPal[];
extern const u16 gFollowingMonPic_EnamorusShinyPal[];
extern const u16 gFollowingMonPic_EnteiShinyPal[];
extern const u16 gFollowingMonPic_EscavalierShinyPal[];
extern const u16 gFollowingMonPic_EspathraShinyPal[];
extern const u16 gFollowingMonPic_EspeonShinyPal[];
extern const u16 gFollowingMonPic_EspurrShinyPal[];
extern const u16 gFollowingMonPic_EternatusShinyPal[];
extern const u16 gFollowingMonPic_ExcadrillShinyPal[];
extern const u16 gFollowingMonPic_ExeggcuteShinyPal[];
extern const u16 gFollowingMonPic_ExeggutorShinyPal[];
extern const u16 gFollowingMonPic_ExploudShinyPal[];
extern const u16 gFollowingMonPic_FalinksShinyPal[];
extern const u16 gFollowingMonPic_FarfetchdShinyPal[];
extern const u16 gFollowingMonPic_FarigirafShinyPal[];
extern const u16 gFollowingMonPic_FearowShinyPal[];
extern const u16 gFollowingMonPic_FeebasShinyPal[];
extern const u16 gFollowingMonPic_FennekinShinyPal[];
extern const u16 gFollowingMonPic_FeraligatrShinyPal[];
extern const u16 gFollowingMonPic_FerroseedShinyPal[];
extern const u16 gFollowingMonPic_FerrothornShinyPal[];
extern const u16 gFollowingMonPic_FezandipitiShinyPal[];
extern const u16 gFollowingMonPic_FidoughShinyPal[];
extern const u16 gFollowingMonPic_FinizenShinyPal[];
extern const u16 gFollowingMonPic_FinneonShinyPal[];
extern const u16 gFollowingMonPic_FlaaffyShinyPal[];
extern const u16 gFollowingMonPic_FlabebeShinyPal[];
extern const u16 gFollowingMonPic_FlamigoShinyPal[];
extern const u16 gFollowingMonPic_FlappleShinyPal[];
extern const u16 gFollowingMonPic_FlareonShinyPal[];
extern const u16 gFollowingMonPic_FletchinderShinyPal[];
extern const u16 gFollowingMonPic_FletchlingShinyPal[];
extern const u16 gFollowingMonPic_FlittleShinyPal[];
extern const u16 gFollowingMonPic_FloatzelShinyPal[];
extern const u16 gFollowingMonPic_FloetteShinyPal[];
extern const u16 gFollowingMonPic_FloragatoShinyPal[];
extern const u16 gFollowingMonPic_FlorgesShinyPal[];
extern const u16 gFollowingMonPic_Flutter_maneShinyPal[];
extern const u16 gFollowingMonPic_FlygonShinyPal[];
extern const u16 gFollowingMonPic_FomantisShinyPal[];
extern const u16 gFollowingMonPic_FoongusShinyPal[];
extern const u16 gFollowingMonPic_ForretressShinyPal[];
extern const u16 gFollowingMonPic_FraxureShinyPal[];
extern const u16 gFollowingMonPic_FrigibaxShinyPal[];
extern const u16 gFollowingMonPic_FrillishShinyPal[];
extern const u16 gFollowingMonPic_FroakieShinyPal[];
extern const u16 gFollowingMonPic_FrogadierShinyPal[];
extern const u16 gFollowingMonPic_FroslassShinyPal[];
extern const u16 gFollowingMonPic_FrosmothShinyPal[];
extern const u16 gFollowingMonPic_FuecocoShinyPal[];
extern const u16 gFollowingMonPic_FurfrouShinyPal[];
extern const u16 gFollowingMonPic_FurretShinyPal[];
extern const u16 gFollowingMonPic_GabiteShinyPal[];
extern const u16 gFollowingMonPic_GalladeShinyPal[];
extern const u16 gFollowingMonPic_GalvantulaShinyPal[];
extern const u16 gFollowingMonPic_GarbodorShinyPal[];
extern const u16 gFollowingMonPic_GarchompShinyPal[];
extern const u16 gFollowingMonPic_GardevoirShinyPal[];
extern const u16 gFollowingMonPic_GarganaclShinyPal[];
extern const u16 gFollowingMonPic_GastlyShinyPal[];
extern const u16 gFollowingMonPic_GastrodonShinyPal[];
extern const u16 gFollowingMonPic_GenesectShinyPal[];
extern const u16 gFollowingMonPic_GengarShinyPal[];
extern const u16 gFollowingMonPic_GeodudeShinyPal[];
extern const u16 gFollowingMonPic_GholdengoShinyPal[];
extern const u16 gFollowingMonPic_GibleShinyPal[];
extern const u16 gFollowingMonPic_GigalithShinyPal[];
extern const u16 gFollowingMonPic_GimmighoulShinyPal[];
extern const u16 gFollowingMonPic_GirafarigShinyPal[];
extern const u16 gFollowingMonPic_GiratinaShinyPal[];
extern const u16 gFollowingMonPic_GlaceonShinyPal[];
extern const u16 gFollowingMonPic_GlalieShinyPal[];
extern const u16 gFollowingMonPic_GlameowShinyPal[];
extern const u16 gFollowingMonPic_GlastrierShinyPal[];
extern const u16 gFollowingMonPic_GligarShinyPal[];
extern const u16 gFollowingMonPic_GlimmetShinyPal[];
extern const u16 gFollowingMonPic_GlimmoraShinyPal[];
extern const u16 gFollowingMonPic_GliscorShinyPal[];
extern const u16 gFollowingMonPic_GloomShinyPal[];
extern const u16 gFollowingMonPic_GogoatShinyPal[];
extern const u16 gFollowingMonPic_GolbatShinyPal[];
extern const u16 gFollowingMonPic_GoldeenShinyPal[];
extern const u16 gFollowingMonPic_GolduckShinyPal[];
extern const u16 gFollowingMonPic_GolemShinyPal[];
extern const u16 gFollowingMonPic_GolettShinyPal[];
extern const u16 gFollowingMonPic_GolisopodShinyPal[];
extern const u16 gFollowingMonPic_GolurkShinyPal[];
extern const u16 gFollowingMonPic_GoodraShinyPal[];
extern const u16 gFollowingMonPic_GoomyShinyPal[];
extern const u16 gFollowingMonPic_GorebyssShinyPal[];
extern const u16 gFollowingMonPic_GossifleurShinyPal[];
extern const u16 gFollowingMonPic_GothitaShinyPal[];
extern const u16 gFollowingMonPic_GothitelleShinyPal[];
extern const u16 gFollowingMonPic_GothoritaShinyPal[];
extern const u16 gFollowingMonPic_Gouging_fireShinyPal[];
extern const u16 gFollowingMonPic_GourgeistShinyPal[];
extern const u16 gFollowingMonPic_GrafaiaiShinyPal[];
extern const u16 gFollowingMonPic_GranbullShinyPal[];
extern const u16 gFollowingMonPic_GrapploctShinyPal[];
extern const u16 gFollowingMonPic_GravelerShinyPal[];
extern const u16 gFollowingMonPic_Great_tuskShinyPal[];
extern const u16 gFollowingMonPic_GreavardShinyPal[];
extern const u16 gFollowingMonPic_GreedentShinyPal[];
extern const u16 gFollowingMonPic_GreninjaShinyPal[];
extern const u16 gFollowingMonPic_GrimerShinyPal[];
extern const u16 gFollowingMonPic_GrimmsnarlShinyPal[];
extern const u16 gFollowingMonPic_GrookeyShinyPal[];
extern const u16 gFollowingMonPic_GrotleShinyPal[];
extern const u16 gFollowingMonPic_GroudonShinyPal[];
extern const u16 gFollowingMonPic_GrovyleShinyPal[];
extern const u16 gFollowingMonPic_GrowlitheShinyPal[];
extern const u16 gFollowingMonPic_GrubbinShinyPal[];
extern const u16 gFollowingMonPic_GrumpigShinyPal[];
extern const u16 gFollowingMonPic_GulpinShinyPal[];
extern const u16 gFollowingMonPic_GumshoosShinyPal[];
extern const u16 gFollowingMonPic_GurdurrShinyPal[];
extern const u16 gFollowingMonPic_GuzzlordShinyPal[];
extern const u16 gFollowingMonPic_GyaradosShinyPal[];
extern const u16 gFollowingMonPic_Hakamo_oShinyPal[];
extern const u16 gFollowingMonPic_HappinyShinyPal[];
extern const u16 gFollowingMonPic_HariyamaShinyPal[];
extern const u16 gFollowingMonPic_HatennaShinyPal[];
extern const u16 gFollowingMonPic_HattereneShinyPal[];
extern const u16 gFollowingMonPic_HattremShinyPal[];
extern const u16 gFollowingMonPic_HaunterShinyPal[];
extern const u16 gFollowingMonPic_HawluchaShinyPal[];
extern const u16 gFollowingMonPic_HaxorusShinyPal[];
extern const u16 gFollowingMonPic_HeatmorShinyPal[];
extern const u16 gFollowingMonPic_HeatranShinyPal[];
extern const u16 gFollowingMonPic_HelioliskShinyPal[];
extern const u16 gFollowingMonPic_HelioptileShinyPal[];
extern const u16 gFollowingMonPic_HeracrossShinyPal[];
extern const u16 gFollowingMonPic_HerdierShinyPal[];
extern const u16 gFollowingMonPic_HippopotasShinyPal[];
extern const u16 gFollowingMonPic_HippowdonShinyPal[];
extern const u16 gFollowingMonPic_HitmonchanShinyPal[];
extern const u16 gFollowingMonPic_HitmonleeShinyPal[];
extern const u16 gFollowingMonPic_HitmontopShinyPal[];
extern const u16 gFollowingMonPic_HonchkrowShinyPal[];
extern const u16 gFollowingMonPic_HonedgeShinyPal[];
extern const u16 gFollowingMonPic_HoopaShinyPal[];
extern const u16 gFollowingMonPic_HoothootShinyPal[];
extern const u16 gFollowingMonPic_HoppipShinyPal[];
extern const u16 gFollowingMonPic_HorseaShinyPal[];
extern const u16 gFollowingMonPic_HoundoomShinyPal[];
extern const u16 gFollowingMonPic_HoundourShinyPal[];
extern const u16 gFollowingMonPic_HoundstoneShinyPal[];
extern const u16 gFollowingMonPic_Ho_ohShinyPal[];
extern const u16 gFollowingMonPic_HuntailShinyPal[];
extern const u16 gFollowingMonPic_HydrappleShinyPal[];
extern const u16 gFollowingMonPic_HydreigonShinyPal[];
extern const u16 gFollowingMonPic_HypnoShinyPal[];
extern const u16 gFollowingMonPic_IgglybuffShinyPal[];
extern const u16 gFollowingMonPic_IllumiseShinyPal[];
extern const u16 gFollowingMonPic_ImpidimpShinyPal[];
extern const u16 gFollowingMonPic_IncineroarShinyPal[];
extern const u16 gFollowingMonPic_IndeedeeShinyPal[];
extern const u16 gFollowingMonPic_InfernapeShinyPal[];
extern const u16 gFollowingMonPic_InkayShinyPal[];
extern const u16 gFollowingMonPic_InteleonShinyPal[];
extern const u16 gFollowingMonPic_Iron_boulderShinyPal[];
extern const u16 gFollowingMonPic_Iron_bundleShinyPal[];
extern const u16 gFollowingMonPic_Iron_crownShinyPal[];
extern const u16 gFollowingMonPic_Iron_handsShinyPal[];
extern const u16 gFollowingMonPic_Iron_jugulisShinyPal[];
extern const u16 gFollowingMonPic_Iron_leavesShinyPal[];
extern const u16 gFollowingMonPic_Iron_mothShinyPal[];
extern const u16 gFollowingMonPic_Iron_thornsShinyPal[];
extern const u16 gFollowingMonPic_Iron_treadsShinyPal[];
extern const u16 gFollowingMonPic_Iron_valiantShinyPal[];
extern const u16 gFollowingMonPic_IvysaurShinyPal[];
extern const u16 gFollowingMonPic_Jangmo_oShinyPal[];
extern const u16 gFollowingMonPic_JellicentShinyPal[];
extern const u16 gFollowingMonPic_JigglypuffShinyPal[];
extern const u16 gFollowingMonPic_JirachiShinyPal[];
extern const u16 gFollowingMonPic_JolteonShinyPal[];
extern const u16 gFollowingMonPic_JoltikShinyPal[];
extern const u16 gFollowingMonPic_JumpluffShinyPal[];
extern const u16 gFollowingMonPic_JynxShinyPal[];
extern const u16 gFollowingMonPic_KabutoShinyPal[];
extern const u16 gFollowingMonPic_KabutopsShinyPal[];
extern const u16 gFollowingMonPic_KadabraShinyPal[];
extern const u16 gFollowingMonPic_KakunaShinyPal[];
extern const u16 gFollowingMonPic_KangaskhanShinyPal[];
extern const u16 gFollowingMonPic_KarrablastShinyPal[];
extern const u16 gFollowingMonPic_KartanaShinyPal[];
extern const u16 gFollowingMonPic_KecleonShinyPal[];
extern const u16 gFollowingMonPic_KeldeoShinyPal[];
extern const u16 gFollowingMonPic_KilowattrelShinyPal[];
extern const u16 gFollowingMonPic_KingambitShinyPal[];
extern const u16 gFollowingMonPic_KingdraShinyPal[];
extern const u16 gFollowingMonPic_KinglerShinyPal[];
extern const u16 gFollowingMonPic_KirliaShinyPal[];
extern const u16 gFollowingMonPic_KlangShinyPal[];
extern const u16 gFollowingMonPic_KlawfShinyPal[];
extern const u16 gFollowingMonPic_KleavorShinyPal[];
extern const u16 gFollowingMonPic_KlefkiShinyPal[];
extern const u16 gFollowingMonPic_KlinkShinyPal[];
extern const u16 gFollowingMonPic_KlinklangShinyPal[];
extern const u16 gFollowingMonPic_KoffingShinyPal[];
extern const u16 gFollowingMonPic_KomalaShinyPal[];
extern const u16 gFollowingMonPic_Kommo_oShinyPal[];
extern const u16 gFollowingMonPic_KoraidonShinyPal[];
extern const u16 gFollowingMonPic_KrabbyShinyPal[];
extern const u16 gFollowingMonPic_KricketotShinyPal[];
extern const u16 gFollowingMonPic_KricketuneShinyPal[];
extern const u16 gFollowingMonPic_KrokorokShinyPal[];
extern const u16 gFollowingMonPic_KrookodileShinyPal[];
extern const u16 gFollowingMonPic_KubfuShinyPal[];
extern const u16 gFollowingMonPic_KyogreShinyPal[];
extern const u16 gFollowingMonPic_KyuremShinyPal[];
extern const u16 gFollowingMonPic_LaironShinyPal[];
extern const u16 gFollowingMonPic_LampentShinyPal[];
extern const u16 gFollowingMonPic_LandorusShinyPal[];
extern const u16 gFollowingMonPic_LanturnShinyPal[];
extern const u16 gFollowingMonPic_LaprasShinyPal[];
extern const u16 gFollowingMonPic_LarvestaShinyPal[];
extern const u16 gFollowingMonPic_LarvitarShinyPal[];
extern const u16 gFollowingMonPic_LatiasShinyPal[];
extern const u16 gFollowingMonPic_LatiosShinyPal[];
extern const u16 gFollowingMonPic_LeafeonShinyPal[];
extern const u16 gFollowingMonPic_LeavannyShinyPal[];
extern const u16 gFollowingMonPic_LechonkShinyPal[];
extern const u16 gFollowingMonPic_LedianShinyPal[];
extern const u16 gFollowingMonPic_LedybaShinyPal[];
extern const u16 gFollowingMonPic_LickilickyShinyPal[];
extern const u16 gFollowingMonPic_LickitungShinyPal[];
extern const u16 gFollowingMonPic_LiepardShinyPal[];
extern const u16 gFollowingMonPic_LileepShinyPal[];
extern const u16 gFollowingMonPic_LilligantShinyPal[];
extern const u16 gFollowingMonPic_LillipupShinyPal[];
extern const u16 gFollowingMonPic_LinooneShinyPal[];
extern const u16 gFollowingMonPic_LitleoShinyPal[];
extern const u16 gFollowingMonPic_LittenShinyPal[];
extern const u16 gFollowingMonPic_LitwickShinyPal[];
extern const u16 gFollowingMonPic_LokixShinyPal[];
extern const u16 gFollowingMonPic_LombreShinyPal[];
extern const u16 gFollowingMonPic_LopunnyShinyPal[];
extern const u16 gFollowingMonPic_LotadShinyPal[];
extern const u16 gFollowingMonPic_LoudredShinyPal[];
extern const u16 gFollowingMonPic_LucarioShinyPal[];
extern const u16 gFollowingMonPic_LudicoloShinyPal[];
extern const u16 gFollowingMonPic_LugiaShinyPal[];
extern const u16 gFollowingMonPic_LumineonShinyPal[];
extern const u16 gFollowingMonPic_LunalaShinyPal[];
extern const u16 gFollowingMonPic_LunatoneShinyPal[];
extern const u16 gFollowingMonPic_LurantisShinyPal[];
extern const u16 gFollowingMonPic_LuvdiscShinyPal[];
extern const u16 gFollowingMonPic_LuxioShinyPal[];
extern const u16 gFollowingMonPic_LuxrayShinyPal[];
extern const u16 gFollowingMonPic_LycanrocShinyPal[];
extern const u16 gFollowingMonPic_MabosstiffShinyPal[];
extern const u16 gFollowingMonPic_MachampShinyPal[];
extern const u16 gFollowingMonPic_MachokeShinyPal[];
extern const u16 gFollowingMonPic_MachopShinyPal[];
extern const u16 gFollowingMonPic_MagbyShinyPal[];
extern const u16 gFollowingMonPic_MagcargoShinyPal[];
extern const u16 gFollowingMonPic_MagearnaShinyPal[];
extern const u16 gFollowingMonPic_MagikarpShinyPal[];
extern const u16 gFollowingMonPic_MagmarShinyPal[];
extern const u16 gFollowingMonPic_MagmortarShinyPal[];
extern const u16 gFollowingMonPic_MagnemiteShinyPal[];
extern const u16 gFollowingMonPic_MagnetonShinyPal[];
extern const u16 gFollowingMonPic_MagnezoneShinyPal[];
extern const u16 gFollowingMonPic_MakuhitaShinyPal[];
extern const u16 gFollowingMonPic_MalamarShinyPal[];
extern const u16 gFollowingMonPic_MamoswineShinyPal[];
extern const u16 gFollowingMonPic_ManaphyShinyPal[];
extern const u16 gFollowingMonPic_MandibuzzShinyPal[];
extern const u16 gFollowingMonPic_ManectricShinyPal[];
extern const u16 gFollowingMonPic_MankeyShinyPal[];
extern const u16 gFollowingMonPic_MantineShinyPal[];
extern const u16 gFollowingMonPic_MantykeShinyPal[];
extern const u16 gFollowingMonPic_MaractusShinyPal[];
extern const u16 gFollowingMonPic_MareanieShinyPal[];
extern const u16 gFollowingMonPic_MareepShinyPal[];
extern const u16 gFollowingMonPic_MarillShinyPal[];
extern const u16 gFollowingMonPic_MarowakShinyPal[];
extern const u16 gFollowingMonPic_MarshadowShinyPal[];
extern const u16 gFollowingMonPic_MarshtompShinyPal[];
extern const u16 gFollowingMonPic_MaschiffShinyPal[];
extern const u16 gFollowingMonPic_MasquerainShinyPal[];
extern const u16 gFollowingMonPic_MausholdShinyPal[];
extern const u16 gFollowingMonPic_MawileShinyPal[];
extern const u16 gFollowingMonPic_MedichamShinyPal[];
extern const u16 gFollowingMonPic_MedititeShinyPal[];
extern const u16 gFollowingMonPic_MeganiumShinyPal[];
extern const u16 gFollowingMonPic_MelmetalShinyPal[];
extern const u16 gFollowingMonPic_MeloettaShinyPal[];
extern const u16 gFollowingMonPic_MeltanShinyPal[];
extern const u16 gFollowingMonPic_MeowscaradaShinyPal[];
extern const u16 gFollowingMonPic_MeowsticShinyPal[];
extern const u16 gFollowingMonPic_MeowthShinyPal[];
extern const u16 gFollowingMonPic_MespritShinyPal[];
extern const u16 gFollowingMonPic_MetagrossShinyPal[];
extern const u16 gFollowingMonPic_MetangShinyPal[];
extern const u16 gFollowingMonPic_MetapodShinyPal[];
extern const u16 gFollowingMonPic_MewShinyPal[];
extern const u16 gFollowingMonPic_MewtwoShinyPal[];
extern const u16 gFollowingMonPic_MienfooShinyPal[];
extern const u16 gFollowingMonPic_MienshaoShinyPal[];
extern const u16 gFollowingMonPic_MightyenaShinyPal[];
extern const u16 gFollowingMonPic_MilceryShinyPal[];
extern const u16 gFollowingMonPic_MiloticShinyPal[];
extern const u16 gFollowingMonPic_MiltankShinyPal[];
extern const u16 gFollowingMonPic_Mime_jrShinyPal[];
extern const u16 gFollowingMonPic_MimikyuShinyPal[];
extern const u16 gFollowingMonPic_MinccinoShinyPal[];
extern const u16 gFollowingMonPic_MiniorShinyPal[];
extern const u16 gFollowingMonPic_MinunShinyPal[];
extern const u16 gFollowingMonPic_MiraidonShinyPal[];
extern const u16 gFollowingMonPic_MisdreavusShinyPal[];
extern const u16 gFollowingMonPic_MismagiusShinyPal[];
extern const u16 gFollowingMonPic_MoltresShinyPal[];
extern const u16 gFollowingMonPic_MonfernoShinyPal[];
extern const u16 gFollowingMonPic_MorelullShinyPal[];
extern const u16 gFollowingMonPic_MorgremShinyPal[];
extern const u16 gFollowingMonPic_MorpekoShinyPal[];
extern const u16 gFollowingMonPic_MothimShinyPal[];
extern const u16 gFollowingMonPic_Mr_mimeShinyPal[];
extern const u16 gFollowingMonPic_Mr_rimeShinyPal[];
extern const u16 gFollowingMonPic_MudbrayShinyPal[];
extern const u16 gFollowingMonPic_MudkipShinyPal[];
extern const u16 gFollowingMonPic_MudsdaleShinyPal[];
extern const u16 gFollowingMonPic_MukShinyPal[];
extern const u16 gFollowingMonPic_MunchlaxShinyPal[];
extern const u16 gFollowingMonPic_MunkidoriShinyPal[];
extern const u16 gFollowingMonPic_MunnaShinyPal[];
extern const u16 gFollowingMonPic_MurkrowShinyPal[];
extern const u16 gFollowingMonPic_MusharnaShinyPal[];
extern const u16 gFollowingMonPic_NacliShinyPal[];
extern const u16 gFollowingMonPic_NaclstackShinyPal[];
extern const u16 gFollowingMonPic_NaganadelShinyPal[];
extern const u16 gFollowingMonPic_NatuShinyPal[];
extern const u16 gFollowingMonPic_NecrozmaShinyPal[];
extern const u16 gFollowingMonPic_NickitShinyPal[];
extern const u16 gFollowingMonPic_NidokingShinyPal[];
extern const u16 gFollowingMonPic_NidoqueenShinyPal[];
extern const u16 gFollowingMonPic_Nidoran_fShinyPal[];
extern const u16 gFollowingMonPic_Nidoran_mShinyPal[];
extern const u16 gFollowingMonPic_NidorinaShinyPal[];
extern const u16 gFollowingMonPic_NidorinoShinyPal[];
extern const u16 gFollowingMonPic_NihilegoShinyPal[];
extern const u16 gFollowingMonPic_NincadaShinyPal[];
extern const u16 gFollowingMonPic_NinetalesShinyPal[];
extern const u16 gFollowingMonPic_NinjaskShinyPal[];
extern const u16 gFollowingMonPic_NoctowlShinyPal[];
extern const u16 gFollowingMonPic_NoibatShinyPal[];
extern const u16 gFollowingMonPic_NoivernShinyPal[];
extern const u16 gFollowingMonPic_NosepassShinyPal[];
extern const u16 gFollowingMonPic_NumelShinyPal[];
extern const u16 gFollowingMonPic_NuzleafShinyPal[];
extern const u16 gFollowingMonPic_NymbleShinyPal[];
extern const u16 gFollowingMonPic_ObstagoonShinyPal[];
extern const u16 gFollowingMonPic_OctilleryShinyPal[];
extern const u16 gFollowingMonPic_OddishShinyPal[];
extern const u16 gFollowingMonPic_OgerponShinyPal[];
extern const u16 gFollowingMonPic_OinkologneShinyPal[];
extern const u16 gFollowingMonPic_OkidogiShinyPal[];
extern const u16 gFollowingMonPic_OmanyteShinyPal[];
extern const u16 gFollowingMonPic_OmastarShinyPal[];
extern const u16 gFollowingMonPic_OnixShinyPal[];
extern const u16 gFollowingMonPic_OranguruShinyPal[];
extern const u16 gFollowingMonPic_OrbeetleShinyPal[];
extern const u16 gFollowingMonPic_OricorioShinyPal[];
extern const u16 gFollowingMonPic_OrthwormShinyPal[];
extern const u16 gFollowingMonPic_OshawottShinyPal[];
extern const u16 gFollowingMonPic_OverqwilShinyPal[];
extern const u16 gFollowingMonPic_PachirisuShinyPal[];
extern const u16 gFollowingMonPic_PalafinShinyPal[];
extern const u16 gFollowingMonPic_PalkiaShinyPal[];
extern const u16 gFollowingMonPic_PalossandShinyPal[];
extern const u16 gFollowingMonPic_PalpitoadShinyPal[];
extern const u16 gFollowingMonPic_PanchamShinyPal[];
extern const u16 gFollowingMonPic_PangoroShinyPal[];
extern const u16 gFollowingMonPic_PanpourShinyPal[];
extern const u16 gFollowingMonPic_PansageShinyPal[];
extern const u16 gFollowingMonPic_PansearShinyPal[];
extern const u16 gFollowingMonPic_ParasShinyPal[];
extern const u16 gFollowingMonPic_ParasectShinyPal[];
extern const u16 gFollowingMonPic_PassimianShinyPal[];
extern const u16 gFollowingMonPic_PatratShinyPal[];
extern const u16 gFollowingMonPic_PawmiShinyPal[];
extern const u16 gFollowingMonPic_PawmoShinyPal[];
extern const u16 gFollowingMonPic_PawmotShinyPal[];
extern const u16 gFollowingMonPic_PawniardShinyPal[];
extern const u16 gFollowingMonPic_PecharuntShinyPal[];
extern const u16 gFollowingMonPic_PelipperShinyPal[];
extern const u16 gFollowingMonPic_PerrserkerShinyPal[];
extern const u16 gFollowingMonPic_PersianShinyPal[];
extern const u16 gFollowingMonPic_PetililShinyPal[];
extern const u16 gFollowingMonPic_PhanpyShinyPal[];
extern const u16 gFollowingMonPic_PhantumpShinyPal[];
extern const u16 gFollowingMonPic_PheromosaShinyPal[];
extern const u16 gFollowingMonPic_PhioneShinyPal[];
extern const u16 gFollowingMonPic_PichuShinyPal[];
extern const u16 gFollowingMonPic_PidgeotShinyPal[];
extern const u16 gFollowingMonPic_PidgeottoShinyPal[];
extern const u16 gFollowingMonPic_PidgeyShinyPal[];
extern const u16 gFollowingMonPic_PidoveShinyPal[];
extern const u16 gFollowingMonPic_PigniteShinyPal[];
extern const u16 gFollowingMonPic_PikachuShinyPal[];
extern const u16 gFollowingMonPic_PikipekShinyPal[];
extern const u16 gFollowingMonPic_PiloswineShinyPal[];
extern const u16 gFollowingMonPic_PincurchinShinyPal[];
extern const u16 gFollowingMonPic_PinecoShinyPal[];
extern const u16 gFollowingMonPic_PinsirShinyPal[];
extern const u16 gFollowingMonPic_PiplupShinyPal[];
extern const u16 gFollowingMonPic_PlusleShinyPal[];
extern const u16 gFollowingMonPic_PoipoleShinyPal[];
extern const u16 gFollowingMonPic_PolitoedShinyPal[];
extern const u16 gFollowingMonPic_PoliwagShinyPal[];
extern const u16 gFollowingMonPic_PoliwhirlShinyPal[];
extern const u16 gFollowingMonPic_PoliwrathShinyPal[];
extern const u16 gFollowingMonPic_PoltchageistShinyPal[];
extern const u16 gFollowingMonPic_PolteageistShinyPal[];
extern const u16 gFollowingMonPic_PonytaShinyPal[];
extern const u16 gFollowingMonPic_PoochyenaShinyPal[];
extern const u16 gFollowingMonPic_PopplioShinyPal[];
extern const u16 gFollowingMonPic_PorygonShinyPal[];
extern const u16 gFollowingMonPic_Porygon2ShinyPal[];
extern const u16 gFollowingMonPic_Porygon_zShinyPal[];
extern const u16 gFollowingMonPic_PrimarinaShinyPal[];
extern const u16 gFollowingMonPic_PrimeapeShinyPal[];
extern const u16 gFollowingMonPic_PrinplupShinyPal[];
extern const u16 gFollowingMonPic_ProbopassShinyPal[];
extern const u16 gFollowingMonPic_PsyduckShinyPal[];
extern const u16 gFollowingMonPic_PumpkabooShinyPal[];
extern const u16 gFollowingMonPic_PupitarShinyPal[];
extern const u16 gFollowingMonPic_PurrloinShinyPal[];
extern const u16 gFollowingMonPic_PuruglyShinyPal[];
extern const u16 gFollowingMonPic_PyroarShinyPal[];
extern const u16 gFollowingMonPic_PyukumukuShinyPal[];
extern const u16 gFollowingMonPic_QuagsireShinyPal[];
extern const u16 gFollowingMonPic_QuaquavalShinyPal[];
extern const u16 gFollowingMonPic_QuaxlyShinyPal[];
extern const u16 gFollowingMonPic_QuaxwellShinyPal[];
extern const u16 gFollowingMonPic_QuilavaShinyPal[];
extern const u16 gFollowingMonPic_QuilladinShinyPal[];
extern const u16 gFollowingMonPic_QwilfishShinyPal[];
extern const u16 gFollowingMonPic_RabootShinyPal[];
extern const u16 gFollowingMonPic_RabscaShinyPal[];
extern const u16 gFollowingMonPic_Raging_boltShinyPal[];
extern const u16 gFollowingMonPic_RaichuShinyPal[];
extern const u16 gFollowingMonPic_RaikouShinyPal[];
extern const u16 gFollowingMonPic_RaltsShinyPal[];
extern const u16 gFollowingMonPic_RampardosShinyPal[];
extern const u16 gFollowingMonPic_RapidashShinyPal[];
extern const u16 gFollowingMonPic_RaticateShinyPal[];
extern const u16 gFollowingMonPic_RattataShinyPal[];
extern const u16 gFollowingMonPic_RayquazaShinyPal[];
extern const u16 gFollowingMonPic_RegiceShinyPal[];
extern const u16 gFollowingMonPic_RegidragoShinyPal[];
extern const u16 gFollowingMonPic_RegielekiShinyPal[];
extern const u16 gFollowingMonPic_RegigigasShinyPal[];
extern const u16 gFollowingMonPic_RegirockShinyPal[];
extern const u16 gFollowingMonPic_RegisteelShinyPal[];
extern const u16 gFollowingMonPic_RelicanthShinyPal[];
extern const u16 gFollowingMonPic_RellorShinyPal[];
extern const u16 gFollowingMonPic_RemoraidShinyPal[];
extern const u16 gFollowingMonPic_ReshiramShinyPal[];
extern const u16 gFollowingMonPic_ReuniclusShinyPal[];
extern const u16 gFollowingMonPic_RevavroomShinyPal[];
extern const u16 gFollowingMonPic_RhydonShinyPal[];
extern const u16 gFollowingMonPic_RhyhornShinyPal[];
extern const u16 gFollowingMonPic_RhyperiorShinyPal[];
extern const u16 gFollowingMonPic_RibombeeShinyPal[];
extern const u16 gFollowingMonPic_RillaboomShinyPal[];
extern const u16 gFollowingMonPic_RioluShinyPal[];
extern const u16 gFollowingMonPic_Roaring_moonShinyPal[];
extern const u16 gFollowingMonPic_RockruffShinyPal[];
extern const u16 gFollowingMonPic_RoggenrolaShinyPal[];
extern const u16 gFollowingMonPic_RolycolyShinyPal[];
extern const u16 gFollowingMonPic_RookideeShinyPal[];
extern const u16 gFollowingMonPic_RoseliaShinyPal[];
extern const u16 gFollowingMonPic_RoseradeShinyPal[];
extern const u16 gFollowingMonPic_RotomShinyPal[];
extern const u16 gFollowingMonPic_RowletShinyPal[];
extern const u16 gFollowingMonPic_RuffletShinyPal[];
extern const u16 gFollowingMonPic_RunerigusShinyPal[];
extern const u16 gFollowingMonPic_SableyeShinyPal[];
extern const u16 gFollowingMonPic_SalamenceShinyPal[];
extern const u16 gFollowingMonPic_SalanditShinyPal[];
extern const u16 gFollowingMonPic_SalazzleShinyPal[];
extern const u16 gFollowingMonPic_SamurottShinyPal[];
extern const u16 gFollowingMonPic_SandacondaShinyPal[];
extern const u16 gFollowingMonPic_SandileShinyPal[];
extern const u16 gFollowingMonPic_SandshrewShinyPal[];
extern const u16 gFollowingMonPic_SandslashShinyPal[];
extern const u16 gFollowingMonPic_SandygastShinyPal[];
extern const u16 gFollowingMonPic_Sandy_shocksShinyPal[];
extern const u16 gFollowingMonPic_SawkShinyPal[];
extern const u16 gFollowingMonPic_SawsbuckShinyPal[];
extern const u16 gFollowingMonPic_ScatterbugShinyPal[];
extern const u16 gFollowingMonPic_SceptileShinyPal[];
extern const u16 gFollowingMonPic_ScizorShinyPal[];
extern const u16 gFollowingMonPic_ScolipedeShinyPal[];
extern const u16 gFollowingMonPic_ScorbunnyShinyPal[];
extern const u16 gFollowingMonPic_ScovillainShinyPal[];
extern const u16 gFollowingMonPic_ScraftyShinyPal[];
extern const u16 gFollowingMonPic_ScraggyShinyPal[];
extern const u16 gFollowingMonPic_Scream_tailShinyPal[];
extern const u16 gFollowingMonPic_ScytherShinyPal[];
extern const u16 gFollowingMonPic_SeadraShinyPal[];
extern const u16 gFollowingMonPic_SeakingShinyPal[];
extern const u16 gFollowingMonPic_SealeoShinyPal[];
extern const u16 gFollowingMonPic_SeedotShinyPal[];
extern const u16 gFollowingMonPic_SeelShinyPal[];
extern const u16 gFollowingMonPic_SeismitoadShinyPal[];
extern const u16 gFollowingMonPic_SentretShinyPal[];
extern const u16 gFollowingMonPic_SerperiorShinyPal[];
extern const u16 gFollowingMonPic_ServineShinyPal[];
extern const u16 gFollowingMonPic_SeviperShinyPal[];
extern const u16 gFollowingMonPic_SewaddleShinyPal[];
extern const u16 gFollowingMonPic_SharpedoShinyPal[];
extern const u16 gFollowingMonPic_ShayminShinyPal[];
extern const u16 gFollowingMonPic_ShedinjaShinyPal[];
extern const u16 gFollowingMonPic_ShelgonShinyPal[];
extern const u16 gFollowingMonPic_ShellderShinyPal[];
extern const u16 gFollowingMonPic_ShellosShinyPal[];
extern const u16 gFollowingMonPic_ShelmetShinyPal[];
extern const u16 gFollowingMonPic_ShieldonShinyPal[];
extern const u16 gFollowingMonPic_ShiftryShinyPal[];
extern const u16 gFollowingMonPic_ShiinoticShinyPal[];
extern const u16 gFollowingMonPic_ShinxShinyPal[];
extern const u16 gFollowingMonPic_ShroodleShinyPal[];
extern const u16 gFollowingMonPic_ShroomishShinyPal[];
extern const u16 gFollowingMonPic_ShuckleShinyPal[];
extern const u16 gFollowingMonPic_ShuppetShinyPal[];
extern const u16 gFollowingMonPic_SigilyphShinyPal[];
extern const u16 gFollowingMonPic_SilcoonShinyPal[];
extern const u16 gFollowingMonPic_SilicobraShinyPal[];
extern const u16 gFollowingMonPic_SilvallyShinyPal[];
extern const u16 gFollowingMonPic_SimipourShinyPal[];
extern const u16 gFollowingMonPic_SimisageShinyPal[];
extern const u16 gFollowingMonPic_SimisearShinyPal[];
extern const u16 gFollowingMonPic_SinistchaShinyPal[];
extern const u16 gFollowingMonPic_SinisteaShinyPal[];
extern const u16 gFollowingMonPic_SirfetchdShinyPal[];
extern const u16 gFollowingMonPic_SizzlipedeShinyPal[];
extern const u16 gFollowingMonPic_SkarmoryShinyPal[];
extern const u16 gFollowingMonPic_SkeledirgeShinyPal[];
extern const u16 gFollowingMonPic_SkiddoShinyPal[];
extern const u16 gFollowingMonPic_SkiploomShinyPal[];
extern const u16 gFollowingMonPic_SkittyShinyPal[];
extern const u16 gFollowingMonPic_SkorupiShinyPal[];
extern const u16 gFollowingMonPic_SkrelpShinyPal[];
extern const u16 gFollowingMonPic_SkuntankShinyPal[];
extern const u16 gFollowingMonPic_SkwovetShinyPal[];
extern const u16 gFollowingMonPic_SlakingShinyPal[];
extern const u16 gFollowingMonPic_SlakothShinyPal[];
extern const u16 gFollowingMonPic_SliggooShinyPal[];
extern const u16 gFollowingMonPic_Slither_wingShinyPal[];
extern const u16 gFollowingMonPic_SlowbroShinyPal[];
extern const u16 gFollowingMonPic_SlowkingShinyPal[];
extern const u16 gFollowingMonPic_SlowpokeShinyPal[];
extern const u16 gFollowingMonPic_SlugmaShinyPal[];
extern const u16 gFollowingMonPic_SlurpuffShinyPal[];
extern const u16 gFollowingMonPic_SmeargleShinyPal[];
extern const u16 gFollowingMonPic_SmolivShinyPal[];
extern const u16 gFollowingMonPic_SmoochumShinyPal[];
extern const u16 gFollowingMonPic_SneaselShinyPal[];
extern const u16 gFollowingMonPic_SneaslerShinyPal[];
extern const u16 gFollowingMonPic_SnivyShinyPal[];
extern const u16 gFollowingMonPic_SnomShinyPal[];
extern const u16 gFollowingMonPic_SnorlaxShinyPal[];
extern const u16 gFollowingMonPic_SnoruntShinyPal[];
extern const u16 gFollowingMonPic_SnoverShinyPal[];
extern const u16 gFollowingMonPic_SnubbullShinyPal[];
extern const u16 gFollowingMonPic_SobbleShinyPal[];
extern const u16 gFollowingMonPic_SolgaleoShinyPal[];
extern const u16 gFollowingMonPic_SolosisShinyPal[];
extern const u16 gFollowingMonPic_SolrockShinyPal[];
extern const u16 gFollowingMonPic_SpearowShinyPal[];
extern const u16 gFollowingMonPic_SpectrierShinyPal[];
extern const u16 gFollowingMonPic_SpewpaShinyPal[];
extern const u16 gFollowingMonPic_SphealShinyPal[];
extern const u16 gFollowingMonPic_SpidopsShinyPal[];
extern const u16 gFollowingMonPic_SpinarakShinyPal[];
extern const u16 gFollowingMonPic_SpindaShinyPal[];
extern const u16 gFollowingMonPic_SpiritombShinyPal[];
extern const u16 gFollowingMonPic_SpoinkShinyPal[];
extern const u16 gFollowingMonPic_SprigatitoShinyPal[];
extern const u16 gFollowingMonPic_SpritzeeShinyPal[];
extern const u16 gFollowingMonPic_SquawkabillyShinyPal[];
extern const u16 gFollowingMonPic_SquirtleShinyPal[];
extern const u16 gFollowingMonPic_StakatakaShinyPal[];
extern const u16 gFollowingMonPic_StantlerShinyPal[];
extern const u16 gFollowingMonPic_StaraptorShinyPal[];
extern const u16 gFollowingMonPic_StaraviaShinyPal[];
extern const u16 gFollowingMonPic_StarlyShinyPal[];
extern const u16 gFollowingMonPic_StarmieShinyPal[];
extern const u16 gFollowingMonPic_StaryuShinyPal[];
extern const u16 gFollowingMonPic_SteelixShinyPal[];
extern const u16 gFollowingMonPic_SteeneeShinyPal[];
extern const u16 gFollowingMonPic_StonjournerShinyPal[];
extern const u16 gFollowingMonPic_StoutlandShinyPal[];
extern const u16 gFollowingMonPic_StuffulShinyPal[];
extern const u16 gFollowingMonPic_StunfiskShinyPal[];
extern const u16 gFollowingMonPic_StunkyShinyPal[];
extern const u16 gFollowingMonPic_SudowoodoShinyPal[];
extern const u16 gFollowingMonPic_SuicuneShinyPal[];
extern const u16 gFollowingMonPic_SunfloraShinyPal[];
extern const u16 gFollowingMonPic_SunkernShinyPal[];
extern const u16 gFollowingMonPic_SurskitShinyPal[];
extern const u16 gFollowingMonPic_SwabluShinyPal[];
extern const u16 gFollowingMonPic_SwadloonShinyPal[];
extern const u16 gFollowingMonPic_SwalotShinyPal[];
extern const u16 gFollowingMonPic_SwampertShinyPal[];
extern const u16 gFollowingMonPic_SwannaShinyPal[];
extern const u16 gFollowingMonPic_SwellowShinyPal[];
extern const u16 gFollowingMonPic_SwinubShinyPal[];
extern const u16 gFollowingMonPic_SwirlixShinyPal[];
extern const u16 gFollowingMonPic_SwoobatShinyPal[];
extern const u16 gFollowingMonPic_SylveonShinyPal[];
extern const u16 gFollowingMonPic_TadbulbShinyPal[];
extern const u16 gFollowingMonPic_TaillowShinyPal[];
extern const u16 gFollowingMonPic_TalonflameShinyPal[];
extern const u16 gFollowingMonPic_TandemausShinyPal[];
extern const u16 gFollowingMonPic_TangelaShinyPal[];
extern const u16 gFollowingMonPic_TangrowthShinyPal[];
extern const u16 gFollowingMonPic_Tapu_buluShinyPal[];
extern const u16 gFollowingMonPic_Tapu_finiShinyPal[];
extern const u16 gFollowingMonPic_Tapu_kokoShinyPal[];
extern const u16 gFollowingMonPic_Tapu_leleShinyPal[];
extern const u16 gFollowingMonPic_TarountulaShinyPal[];
extern const u16 gFollowingMonPic_TatsugiriShinyPal[];
extern const u16 gFollowingMonPic_TaurosShinyPal[];
extern const u16 gFollowingMonPic_TeddiursaShinyPal[];
extern const u16 gFollowingMonPic_TentacoolShinyPal[];
extern const u16 gFollowingMonPic_TentacruelShinyPal[];
extern const u16 gFollowingMonPic_TepigShinyPal[];
extern const u16 gFollowingMonPic_TerapagosShinyPal[];
extern const u16 gFollowingMonPic_TerrakionShinyPal[];
extern const u16 gFollowingMonPic_ThievulShinyPal[];
extern const u16 gFollowingMonPic_ThrohShinyPal[];
extern const u16 gFollowingMonPic_ThundurusShinyPal[];
extern const u16 gFollowingMonPic_ThwackeyShinyPal[];
extern const u16 gFollowingMonPic_TimburrShinyPal[];
extern const u16 gFollowingMonPic_Ting_luShinyPal[];
extern const u16 gFollowingMonPic_TinkatinkShinyPal[];
extern const u16 gFollowingMonPic_TinkatonShinyPal[];
extern const u16 gFollowingMonPic_TinkatuffShinyPal[];
extern const u16 gFollowingMonPic_TirtougaShinyPal[];
extern const u16 gFollowingMonPic_ToedscoolShinyPal[];
extern const u16 gFollowingMonPic_ToedscruelShinyPal[];
extern const u16 gFollowingMonPic_TogedemaruShinyPal[];
extern const u16 gFollowingMonPic_TogekissShinyPal[];
extern const u16 gFollowingMonPic_TogepiShinyPal[];
extern const u16 gFollowingMonPic_TogeticShinyPal[];
extern const u16 gFollowingMonPic_TorchicShinyPal[];
extern const u16 gFollowingMonPic_TorkoalShinyPal[];
extern const u16 gFollowingMonPic_TornadusShinyPal[];
extern const u16 gFollowingMonPic_TorracatShinyPal[];
extern const u16 gFollowingMonPic_TorterraShinyPal[];
extern const u16 gFollowingMonPic_TotodileShinyPal[];
extern const u16 gFollowingMonPic_ToucannonShinyPal[];
extern const u16 gFollowingMonPic_ToxapexShinyPal[];
extern const u16 gFollowingMonPic_ToxelShinyPal[];
extern const u16 gFollowingMonPic_ToxicroakShinyPal[];
extern const u16 gFollowingMonPic_ToxtricityShinyPal[];
extern const u16 gFollowingMonPic_TranquillShinyPal[];
extern const u16 gFollowingMonPic_TrapinchShinyPal[];
extern const u16 gFollowingMonPic_TreeckoShinyPal[];
extern const u16 gFollowingMonPic_TrevenantShinyPal[];
extern const u16 gFollowingMonPic_TropiusShinyPal[];
extern const u16 gFollowingMonPic_TrubbishShinyPal[];
extern const u16 gFollowingMonPic_TrumbeakShinyPal[];
extern const u16 gFollowingMonPic_TsareenaShinyPal[];
extern const u16 gFollowingMonPic_TurtonatorShinyPal[];
extern const u16 gFollowingMonPic_TurtwigShinyPal[];
extern const u16 gFollowingMonPic_TympoleShinyPal[];
extern const u16 gFollowingMonPic_TynamoShinyPal[];
extern const u16 gFollowingMonPic_Type_nullShinyPal[];
extern const u16 gFollowingMonPic_TyphlosionShinyPal[];
extern const u16 gFollowingMonPic_TyranitarShinyPal[];
extern const u16 gFollowingMonPic_TyrantrumShinyPal[];
extern const u16 gFollowingMonPic_TyrogueShinyPal[];
extern const u16 gFollowingMonPic_TyruntShinyPal[];
extern const u16 gFollowingMonPic_UmbreonShinyPal[];
extern const u16 gFollowingMonPic_UnfezantShinyPal[];
extern const u16 gFollowingMonPic_UnownShinyPal[];
extern const u16 gFollowingMonPic_UrsalunaShinyPal[];
extern const u16 gFollowingMonPic_UrsaringShinyPal[];
extern const u16 gFollowingMonPic_UrshifuShinyPal[];
extern const u16 gFollowingMonPic_UxieShinyPal[];
extern const u16 gFollowingMonPic_VanillishShinyPal[];
extern const u16 gFollowingMonPic_VanilliteShinyPal[];
extern const u16 gFollowingMonPic_VanilluxeShinyPal[];
extern const u16 gFollowingMonPic_VaporeonShinyPal[];
extern const u16 gFollowingMonPic_VaroomShinyPal[];
extern const u16 gFollowingMonPic_VeluzaShinyPal[];
extern const u16 gFollowingMonPic_VenipedeShinyPal[];
extern const u16 gFollowingMonPic_VenomothShinyPal[];
extern const u16 gFollowingMonPic_VenonatShinyPal[];
extern const u16 gFollowingMonPic_VenusaurShinyPal[];
extern const u16 gFollowingMonPic_VespiquenShinyPal[];
extern const u16 gFollowingMonPic_VibravaShinyPal[];
extern const u16 gFollowingMonPic_VictiniShinyPal[];
extern const u16 gFollowingMonPic_VictreebelShinyPal[];
extern const u16 gFollowingMonPic_VigorothShinyPal[];
extern const u16 gFollowingMonPic_VikavoltShinyPal[];
extern const u16 gFollowingMonPic_VileplumeShinyPal[];
extern const u16 gFollowingMonPic_VirizionShinyPal[];
extern const u16 gFollowingMonPic_VivillonShinyPal[];
extern const u16 gFollowingMonPic_VolbeatShinyPal[];
extern const u16 gFollowingMonPic_VolcanionShinyPal[];
extern const u16 gFollowingMonPic_VolcaronaShinyPal[];
extern const u16 gFollowingMonPic_VoltorbShinyPal[];
extern const u16 gFollowingMonPic_VullabyShinyPal[];
extern const u16 gFollowingMonPic_VulpixShinyPal[];
extern const u16 gFollowingMonPic_WailmerShinyPal[];
extern const u16 gFollowingMonPic_WailordShinyPal[];
extern const u16 gFollowingMonPic_Walking_wakeShinyPal[];
extern const u16 gFollowingMonPic_WalreinShinyPal[];
extern const u16 gFollowingMonPic_WartortleShinyPal[];
extern const u16 gFollowingMonPic_WatchogShinyPal[];
extern const u16 gFollowingMonPic_WattrelShinyPal[];
extern const u16 gFollowingMonPic_WeavileShinyPal[];
extern const u16 gFollowingMonPic_WeedleShinyPal[];
extern const u16 gFollowingMonPic_WeepinbellShinyPal[];
extern const u16 gFollowingMonPic_WeezingShinyPal[];
extern const u16 gFollowingMonPic_WhimsicottShinyPal[];
extern const u16 gFollowingMonPic_WhirlipedeShinyPal[];
extern const u16 gFollowingMonPic_WhiscashShinyPal[];
extern const u16 gFollowingMonPic_WhismurShinyPal[];
extern const u16 gFollowingMonPic_WigglytuffShinyPal[];
extern const u16 gFollowingMonPic_WiglettShinyPal[];
extern const u16 gFollowingMonPic_WimpodShinyPal[];
extern const u16 gFollowingMonPic_WingullShinyPal[];
extern const u16 gFollowingMonPic_WishiwashiShinyPal[];
extern const u16 gFollowingMonPic_WobbuffetShinyPal[];
extern const u16 gFollowingMonPic_WoobatShinyPal[];
extern const u16 gFollowingMonPic_WoolooShinyPal[];
extern const u16 gFollowingMonPic_WooperShinyPal[];
extern const u16 gFollowingMonPic_WormadamShinyPal[];
extern const u16 gFollowingMonPic_Wo_chienShinyPal[];
extern const u16 gFollowingMonPic_WugtrioShinyPal[];
extern const u16 gFollowingMonPic_WurmpleShinyPal[];
extern const u16 gFollowingMonPic_WynautShinyPal[];
extern const u16 gFollowingMonPic_WyrdeerShinyPal[];
extern const u16 gFollowingMonPic_XatuShinyPal[];
extern const u16 gFollowingMonPic_XerneasShinyPal[];
extern const u16 gFollowingMonPic_XurkitreeShinyPal[];
extern const u16 gFollowingMonPic_YamaskShinyPal[];
extern const u16 gFollowingMonPic_YamperShinyPal[];
extern const u16 gFollowingMonPic_YanmaShinyPal[];
extern const u16 gFollowingMonPic_YanmegaShinyPal[];
extern const u16 gFollowingMonPic_YungoosShinyPal[];
extern const u16 gFollowingMonPic_YveltalShinyPal[];
extern const u16 gFollowingMonPic_ZacianShinyPal[];
extern const u16 gFollowingMonPic_ZamazentaShinyPal[];
extern const u16 gFollowingMonPic_ZangooseShinyPal[];
extern const u16 gFollowingMonPic_ZapdosShinyPal[];
extern const u16 gFollowingMonPic_ZarudeShinyPal[];
extern const u16 gFollowingMonPic_ZebstrikaShinyPal[];
extern const u16 gFollowingMonPic_ZekromShinyPal[];
extern const u16 gFollowingMonPic_ZeraoraShinyPal[];
extern const u16 gFollowingMonPic_ZigzagoonShinyPal[];
extern const u16 gFollowingMonPic_ZoroarkShinyPal[];
extern const u16 gFollowingMonPic_ZoruaShinyPal[];
extern const u16 gFollowingMonPic_ZubatShinyPal[];
extern const u16 gFollowingMonPic_ZweilousShinyPal[];
extern const u16 gFollowingMonPic_ZygardeShinyPal[];
extern const u16 gFollowingMonPic_PonytaGalarianShinyPal[];
extern const u16 gFollowingMonPic_QwilfishHisuianShinyPal[];
extern const u16 gFollowingMonPic_RaichuAlolanShinyPal[];
extern const u16 gFollowingMonPic_RapidashGalarianShinyPal[];
extern const u16 gFollowingMonPic_RaticateAlolanShinyPal[];
extern const u16 gFollowingMonPic_RattataAlolanShinyPal[];
extern const u16 gFollowingMonPic_RotomFanShinyPal[];
extern const u16 gFollowingMonPic_RotomFrostShinyPal[];
extern const u16 gFollowingMonPic_RotomHeatShinyPal[];
extern const u16 gFollowingMonPic_RotomMowShinyPal[];
extern const u16 gFollowingMonPic_RotomWashShinyPal[];
extern const u16 gFollowingMonPic_SamurottHisuianShinyPal[];
extern const u16 gFollowingMonPic_SandshrewAlolanShinyPal[];
extern const u16 gFollowingMonPic_SandslashAlolanShinyPal[];
extern const u16 gFollowingMonPic_SawsbuckAutumnShinyPal[];
extern const u16 gFollowingMonPic_SawsbuckSummerShinyPal[];
extern const u16 gFollowingMonPic_SawsbuckWinterShinyPal[];
extern const u16 gFollowingMonPic_ShayminSkyShinyPal[];
extern const u16 gFollowingMonPic_ShellosEast_seaShinyPal[];
extern const u16 gFollowingMonPic_SliggooHisuianShinyPal[];
extern const u16 gFollowingMonPic_SlowbroGalarianShinyPal[];
extern const u16 gFollowingMonPic_SlowkingGalarianShinyPal[];
extern const u16 gFollowingMonPic_SlowpokeGalarianShinyPal[];
extern const u16 gFollowingMonPic_SneaselHisuianShinyPal[];
extern const u16 gFollowingMonPic_SquawkabillyBlue_plumageShinyPal[];
extern const u16 gFollowingMonPic_SquawkabillyWhite_plumageShinyPal[];
extern const u16 gFollowingMonPic_SquawkabillyYellow_plumageShinyPal[];
extern const u16 gFollowingMonPic_StunfiskGalarianShinyPal[];
extern const u16 gFollowingMonPic_TatsugiriDroopyShinyPal[];
extern const u16 gFollowingMonPic_TatsugiriStretchyShinyPal[];
extern const u16 gFollowingMonPic_TaurosPaldean_aqua_breedShinyPal[];
extern const u16 gFollowingMonPic_TaurosPaldean_blaze_breedShinyPal[];
extern const u16 gFollowingMonPic_TaurosPaldean_combat_breedShinyPal[];
extern const u16 gFollowingMonPic_TerapagosTerastalShinyPal[];
extern const u16 gFollowingMonPic_ThundurusTherianShinyPal[];
extern const u16 gFollowingMonPic_TornadusTherianShinyPal[];
extern const u16 gFollowingMonPic_ToxtricityLow_keyShinyPal[];
extern const u16 gFollowingMonPic_TyphlosionHisuianShinyPal[];
extern const u16 gFollowingMonPic_UrsalunaBloodmoonShinyPal[];
extern const u16 gFollowingMonPic_VivillonArchipelagoShinyPal[];
extern const u16 gFollowingMonPic_VivillonContinentalShinyPal[];
extern const u16 gFollowingMonPic_VivillonElegantShinyPal[];
extern const u16 gFollowingMonPic_VivillonFancyShinyPal[];
extern const u16 gFollowingMonPic_VivillonGardenShinyPal[];
extern const u16 gFollowingMonPic_VivillonHigh_plainsShinyPal[];
extern const u16 gFollowingMonPic_VivillonJungleShinyPal[];
extern const u16 gFollowingMonPic_VivillonMarineShinyPal[];
extern const u16 gFollowingMonPic_VivillonMeadowShinyPal[];
extern const u16 gFollowingMonPic_VivillonModernShinyPal[];
extern const u16 gFollowingMonPic_VivillonMonsoonShinyPal[];
extern const u16 gFollowingMonPic_VivillonOceanShinyPal[];
extern const u16 gFollowingMonPic_VivillonPoke_ballShinyPal[];
extern const u16 gFollowingMonPic_VivillonPolarShinyPal[];
extern const u16 gFollowingMonPic_VivillonRiverShinyPal[];
extern const u16 gFollowingMonPic_VivillonSandstormShinyPal[];
extern const u16 gFollowingMonPic_VivillonSavannaShinyPal[];
extern const u16 gFollowingMonPic_VivillonSunShinyPal[];
extern const u16 gFollowingMonPic_VivillonTundraShinyPal[];
extern const u16 gFollowingMonPic_VoltorbHisuianShinyPal[];
extern const u16 gFollowingMonPic_VulpixAlolanShinyPal[];
extern const u16 gFollowingMonPic_WeezingGalarianShinyPal[];
extern const u16 gFollowingMonPic_WooperWooper_paldeanShinyPal[];
extern const u16 gFollowingMonPic_WormadamSandy_cloakShinyPal[];
extern const u16 gFollowingMonPic_WormadamTrash_cloakShinyPal[];
extern const u16 gFollowingMonPic_XerneasActiveShinyPal[];
extern const u16 gFollowingMonPic_YamaskGalarianShinyPal[];
extern const u16 gFollowingMonPic_ZacianCrowned_swordShinyPal[];
extern const u16 gFollowingMonPic_ZamazentaCrowned_shieldShinyPal[];
extern const u16 gFollowingMonPic_ZapdosGalarianShinyPal[];
extern const u16 gFollowingMonPic_ZigzagoonGalarianShinyPal[];
extern const u16 gFollowingMonPic_ZoroarkHisuianShinyPal[];
extern const u16 gFollowingMonPic_ZoruaHisuianShinyPal[];
extern const u16 gFollowingMonPic_Zygarde10_percentShinyPal[];
extern const u16 gFollowingMonPic_ZygardeCompleteShinyPal[];
extern const u16 gFollowingMonPic_Unfezant_FNormalPal[];


#endif