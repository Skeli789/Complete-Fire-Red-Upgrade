/* New Things */
ExtensionState = 0x3000F28;

/*Old Things*/
gEvolutionTable = 0x08989D30; /*Change this*/
/*gBattleScriptsForMoveEffects = 0x8903B70; Remove This*/

gAccuracyStageRatios = 0x82504FC;
sTurnActionsFuncsTable = 0x8250038;
sEndTurnFuncsTable = 0x8250070;
sTargetIdentities = 0x8250980;
sBattleIntroSlideFuncs = 0x83E7CD4;

gMapHeader = 0x2036DFC;
gMPlay_BGM = 0x3007300;

/* Variables */
var_8003 = 0x020370BE;
var_8004 = 0x020370C0;
var_8005 = 0x020370C2;
var_8006 = 0x020370C4;
var_800D_lastresult = 0x20370D0;
var_800E = 0x0203AD30;

__udivsi3 = 0x081E460C|1;
__umodsi3 = 0x081E4684|1;
__divsi3 = 0x081E4018|1;

/* Attributes */
get_attr = 0x0803FBE8|1;
set_attr = 0x0804037C|1;

/* Pokemon */
pokemon_slot_purge = 0x0803D994|1;
count_pokemon = 0x02024029;
party_player = 0x02024284;

calculate_stats_pokemon = 0x0803E47C|1;

get_poke_ability = 0x08040D46|1; 
species_to_dex_index = 0x08043298|1;
givepokemon = 0x080A011C|1;
party_move_up_no_free_slots_in_between = 0x080937DC|1;

/*items*/
routine_to_run_after_graphics = 0x02039998;
run_after_graphics = 0x080A103C|1;
script_env_2_enable = 0x08069940|1;
script_run = 0x08069AE4|1;
get_item_x12_battle_function = 0x0809A924|1;
get_item_lastword = 0x0809AA8C|1;
get_item_quality = 0x0809A949|1;
item_is_mail = 0x080980F8|1;
prepare_party_mail_to_give = 0x08097D38|1;
bag_add_item = 0x0809A084|1;

/* move */
teach_move_in_available_slot = 0x0803E8B0|1;
Special_DD_delete_move = 0x080BFBA8|1;

/* Strings */
str_cpy = 0x08008D84|1;

/*
 * Data
 */
 
obj_set_rot = 0x08075858|1;
sub_813A45C = 0x0813A45C|1;
sub_80BF77C = 0x080BF77C|1;
rboxes = 0x20204B4;
ball_tiles = 0x0826056C;
ball_palettes = 0x082605CC;
gpu_pal_decompress_alloc_tag_and_upload = 0x0800F078|1;
obj_sync_something = 0x08006BA8|1;
objc_ball_instanciate2_entry_804B9E8 = 0x0804B9E8|1;
sub_804BF64 = 0x804BF64|1;
super_sprites_delete_range = 0x080072E8|1;
sub_8094AD8 = 0x08094AD8|1;
affine_coefficients = 0x2021BCC;
buffer1 = 0x02022100;
buffer2 = 0x02021CD0;
buffer3 = 0x02021CF0;
buffer4 = 0x02021D04;
string_buffer = 0x02021D18;
format_name_and_gender_string = 0x080496C0|1;
sub_8049D10 = 0x08049D10|1;
healthbar_draw_hp = 0x08048440|1;
pokemon_get_gender = 0x0803F720|1;
sub_804A6D8 = 0x0804A6D8|1;
sub_804A6E8 = 0x0804A6E8|1;
rbox_get_id_something = 0x08003CE4|1;
sub_812E5A4 = 0x0812E5A4|1;
rboxid_get_field = 0x08004950|1;

gf_strcat = 0x08008DA4 |1;

healthbar_draw_level = 0x08048398|1;
gpu_pal_obj_alloc_tag_and_apply = 0x08008928|1;
gpu_tile_obj_decompress_alloc_tag_and_upload = 0x0800F034|1;
template_instanciate_reverse_search = 0x08006FE0|1;
palc_health_boxes = 0x08260260|1;
tester_run = 0x080347A8|1;
healthbar_draw_field_maybe = 0x08049D98|1;

enemyYTable = 0x082349CC;
playerYTable = 0x08235E6C;

set_enemy_y = 0x08076494|1;

pkmn_names_table = 0x08245EE0;
move_names = 0x8247094;
ability_names = 0x0824FC40;
ai_trainer = 0x0823EAC8; /* trainer table */
trainer_class_names = 0x0823E558;
typenames = 0x0824F1A0;
items = 0x083DB028;

vblank_cb_battle_BG0HOFS = 0x02022974;
vblank_cb_battle_BG0VOFS = 0x02022976;
vblank_cb_battle_BG1HOFS = 0x02022978;
vblank_cb_battle_BG1VOFS = 0x0202297A;
vblank_cb_battle_BG2HOFS = 0x0202297C;
vblank_cb_battle_BG2VOFS = 0x0202297E;
vblank_cb_battle_BG3HOFS = 0x02022980;
vblank_cb_battle_BG3VOFS = 0x02022982;
vblank_cb_battle_WIN0H = 0x02022984;
vblank_cb_battle_WIN0V = 0x02022986;
vblank_cb_battle_WIN1H = 0x02022988;
vblank_cb_battle_WIN1V = 0x0202298A;
dp12_8700 = 0x02038700;

dp12_config = 0x02039600;
script_env_1 = 0x03000EB0;
script_env_2 = 0x03000F28;
pal_fade_control = 0x02037AB8;
map_post_load_hook = 0x03005020;
tasks = 0x03005090;
objects = 0x0202063C;
poke_oam_battle = 0x0824F018;
anim_poke = 0x082349BC;
rotscale_empty = 0x08231CFC;
oac_nullsub = 0x0810BB89;
obj_ids_to_display = 0x02021800;
evolution_table = 0x08259754;
pokemon_bank = 0x0202402C;
base_stats = 0x08254784;
dex_data = 0x0844E850;
battle_env_table = 0x0824EE34;
battle_data_ptrs = 0x0203BFFC;
battle_load_global = 0x0203F000; /*Free RAM space*/
superstate = 0x030030F0;

textflags = 0x03003E50;
saveblock2 = 0x0300500C;
objt_pokemon = 0x020244DC;
animtable1_pokemon = 0x082349BC|1;
template_0203AD40 = 0x0203AD40;
uns_table_pokemon_trainer = 0x0825DF50;
meta_animtable_trainer = 0x08239F74;
gfx_table_trainer_back = 0x08239FA4;
gfx_table_trainer_front = 0x0823957C;
gfx_table_pokemon_front = 0x082350AC;
gfx_table_pokemon_back = 0x0823654C;
backsprite_pals = 0x08239FD4;
animtable1_front = 0x082390DC;
animtable1_pokemon = 0x82349BC;
oac_poke_opponent = 0x08011D64|1;
oam_poke_ally = 0x0824F018;
pal_table_nonshiny = 0x0823730C;
pal_table_shiny = 0x082380CC;
pal_table_trainer_front = 0x08239A1C;
pal_table_trainer_back = 0x08239FD4;
anim_082347F8 = 0x082347F8;
ball_templates = 0x082606F4;
enemy_y_table = 0x082349cc;
altitude_table = 0x0823A004;
gfx_altitude = 0x08E7BAD4;
pal_altitude = 0x08D11B84;


/*
* Place holder data
*/

berries_t = 0x8740000;
sheer_force_t = 0x8740000;
mega_items_t = 0x8740000;
megas = 0x8740000;
type_chart = 0x8740000;
battle_strings = 0x8740000;
battle_mallocd_resources = 0x0203C000;

obj_delete_and_free_associated_resources = 0x08007804|1;

/*
 * Functions
 */

bs_exec = 0x080123E4|1;
b_setup_bx = 0x0800D30C|1;
cpu_set = 0x081E3B64|1;
load_battle_screen_elements2 = 0x0800F2AC|1;
vblank_cb_battle = 0x080116F4|1;
sub_8001960 = 0x08001960|1;
sub_80357C8 = 0x080357C8|1;
gpu_pal_apply = 0x080703EC|1;

remoboxes_upload_tilesets = 0x08002DE8|1;
pal_fade_control_reset_maybe = 0x08070A84|1;
fmt_int_10 = 0x08008E78|1;
tasks_init = 0x080773BC|1;
gpu_pal_allocator_reset = 0x080088F0|1;
gpu_tile_obj_decompress_alloc_tag_and_upload = 0x0800F035;
gpu_pal_obj_alloc_tag_and_apply = 0x08008929;
oac_nullsub_0 = 0x810BB88|1;
rboxid_clear_pixels = 0x0800445C|1;
rboxid_08003FA0 =  0x08003FA0|1;
rboxid_to_vram = 0x08003F20|1;
something_08050DE1 = 0x08050DE0|1;
gpu_tile_obj_alloc_tag_and_upload = 0x080086DC|1;
gpu_pal_obj_alloc_tag_and_apply = 0x08008928|1;
gpu_sync_bg_show = 0x080019BC|1;
something_08050969 = 0x08050968|1;
box_print = 0x0812E51C|1;
sub_0800F380 = 0x0800F380|1;
sub_0800F34C = 0x0800F34C|1;
sub_0800F420 = 0x0800F420|1;
rboxid_tilemap_update = 0x08003FA0|1;
rboxid_upload_a = 0x08003F20|1;
sub_8002C48 = 0x08002C48|1;
sub_8150048 = 0x08150048|1;
battle_show_message = 0x080D87BC|1;
remoid_a_pressed_maybe = 0x08002E64|1;

c2_exit_to_overworld_1_continue_scripts_and_music = 0x080568C4|1;
palette_bg_faded_fill_black = 0x0807DB38 |1;
overworld_free_bgmaps = 0x080563F0|1;
lcd_io_set = 0x08000A38|1;
lcd_io_get = 0x08000AC4|1;
vblank_handler_set = 0x080006F4|1;
hblank_handler_set = 0x08000700|1;
gpu_tile_bg_drop_all_sets = 0x08001618|1;
bg_positions_reset = 0x0812D594|1;
callback_clear_and_init = 0x080F4E24|1;
bgid_nullify_tilemap = 0x08001FD4|1;
mapldr_continue_scripts_restart_music = 0x0807DD24|1;
c2_exit_to_overworld_2_switch = 0x080567DC|1;
gpu_sprites_upload = 0x08007320|1;
copy_queue_process = 0x08007610|1;
gpu_pal_upload = 0x08070474|1;
bg_scroll_invert = 0x08087F54|1;
dp12_reset_something = 0x08087EA4|1;
dp12_something = 0x08087EE4|1;
sub_8087EE4 = 0x08087EE4|1;
dp12_update = 0x08087F54|1;
dp12_b_callback = 0x08087FE0|1;
dp12_abort = 0x08087E64|1;
pal_decompress_slice_to_faded_and_unfaded = 0x080703A8|1;
get_bs_elem_env_index = 0x0800FC2C|1;

obj_and_aux_reset_all = 0x08006B10|1;

load_var_value = 0x0806E568|1;
var_set_value = 0x0806E584|1;
var_get_pointer = 0x0806E454|1;
memcpy_ = 0x081E5E78|1;
Memcpy = 0x081E5E78|1;
malloc_ = 0x08002B9C|1;
calloc_ = 0x8002BB0 | 1;
malloc_and_clear = 0x08002BB0|1;
free_ = 0x08002BC4|1;
task_add = 0x0807741C|1;
task_delete = 0x08077508|1;
set_callback2 = 0x08000544|1;
set_callback1 = 0x080565E0|1;
fade_screen = 0x08070588|1;
sp198_help_system_disable = 0x0812B478|1;
malloc_and_LZ77UnComp = 0x080F6AA0|1;
gpu_copy_to_tileset = 0x080017D0|1;
gpu_copy_tilemap = 0x08002040|1;
bgid_send_tilemap = 0x080020BC|1;
gpu_pal_apply = 0x080703EC|1;
lz77_uncomp_vram = 0x081E3B6C|1;
lz77_uncomp_wram = 0x081E3B70|1;

trainer_card_reset = 0x08089D0C|1;

task_exec = 0x08077578|1;
textbox_something = 0x08002DE8|1;
objc_exec = 0x08006B5C|1;
obj_sync_something = 0x08006BA8|1;
fade_and_return_progress_probably = 0x080704D0|1;
decompress_with_fallback_maybe = 0x080F6878|1;
gpu_sync_bg_show = 0x080019BC|1;
gpu_sync_bg_hide = 0x080019D0|1;
bgid_mod_x_offset = 0x08001B90|1;
bgid_mod_y_offset = 0x08001D08|1;
gpu_pal_alloc_new = 0x080089B8|1;

bgid_set_tilemap = 0x08001FA0|1;
bg_vram_setup = 0x08001658|1;
memset_ = 0x081E5ED8|1;
Memset = 0x081E5ED8|1;


template_instanciate_forward_search = 0x08006F8C|1;
obj_delete_and_free_tiles = 0x080073DC|1;
sub_08034750 = 0x08034750|1;
template_build_for_pokemon_or_trainer = 0x0803F864|1;
obj_anim_image_start = 0x0800838C|1;
obj_delete_and_free_tiles_and_free_tiles = 0x08007280|1;
obj_delete = 0x080073DC|1;
object_new_hidden_with_callback = 0x08044EF8|1;
sub_8037A2A = 0x08038C80|1;
sub_0804B908 = 0x0804B908|1;


create_pokemon = 0x0803DD98|1;

divmod_remainder = 0x081E4684|1;
rand = 0x08044EC8|1;
__aeabi_idiv = 0x081E4684|1;
__aeabi_uidiv = 0x081E4684|1;
sub_8037A28 = 0x08037A28|1;
species_to_dex_index = 0x08043298|1;

strcpy_xFF_terminated = 0x08008D84|1;
sub_080F6EE4 = 0x080F6EE4|1;
box_related_one = 0x080F6CD0|1;
sav2_get_text_speed = 0x080F78A8|1;
rboxid_to_vram = 0x08003F20|1;
sub_81220D4 = 0x081220D4|1;
box_curved = 0x080F7768|1;
sub_080F696C = 0x080F696C|1;
sub_08002040 = 0x08002040|1;
gpu_sync_bg_show = 0x80019BC|1;
LZ77UnCompAnyRAM = 0x0800EC8C|1;
oamt_spawn_poke_or_trainer_picture = 0x0810BDAC|1;
uns_builder_assign_animtable1 = 0x0810BD80|1;
ball_number_to_ball_processing_index = 0x080EF52C|1;
ball_to_ram_setup = 0x0804BEF4|1;
dp13_810BB8C = 0x0810BB8C|1;
battle_load_health_boxes_unused = 0x080347A8|1;
some_test_func = 0x080F1198|1;
init_malloc = 0x08002B80|1;

pal_fade_control_and_dead_struct_reset = 0x08070528|1;

/*
 * IO
 */

BG3HOFS = 0x0400001C;
DMA0_SRC = 0x040000B0;
DMA0_DST = 0x040000B4;
DMA0_CNT_L = 0x040000B8;
DMA0_CNT_H = 0x040000BA;

/*
 * Custom Data
 */

interpreter_state = 0x02036E30;

/*PokeFireRed Function Names*/

/*Battle Functions*/
StringGetEnd10 = 0x8008D28 | 1;
GetExpandedPlaceholder = 0x80091E0 | 1;
GetMultiplayerId = 0x800A404 | 1;
PrepareBufferDataTransfer = 0x800D8B0 | 1;
PrepareBufferDataTransferLink = 0x800D9EC | 1;
BtlController_EmitSetMonData = 0x800DFF0 | 1;
EmitTrainerThrow = 0x800E0D4 | 1;
EmitTrainerSlide = 0x800E0F4 | 1;
EmitTrainerSlideBack = 0x800E114 | 1;
EmitBallThrow = 0x800E194 | 1;
EmitMoveAnimation = 0x800E1FC | 1;
EmitPrintString = 0x800E2D4 | 1;
EmitHealthBarUpdate = 0x800E5EC | 1;
/*EmitExpBarUpdate = 0x800E624 | 1;*/
EmitStatusAnimation = 0x800E6AC | 1;
EmitTwoReturnValues = 0x800E848 | 1;
EmitChosenMonReturnValue = 0x800E874 | 1;
EmitTrainerBallThrow = 0x800EA5C | 1;
EmitDrawPartyStatusSummary = 0x800EA7C | 1;
EmitSpriteInvisibility = 0x800EB04 | 1;
EmitBattleAnimation = 0x800EB28 | 1;
LoadBattleBG_SpecialTerrainID = 0x800F238 | 1;
sub_8012044 = 0x8012044 | 1;
sub_8012098 = 0x8012098 | 1;
DoBounceEffect = 0x8012138 | 1;
EndBounceEffect = 0x8012258 | 1;
SwitchInClearSetData = 0x8012760 | 1;
UndoEffectsAfterFainting = 0x8012BC8 | 1;
BattleIntroRecordMonsToDex = 0x801362C | 1;
sub_8013F6C = 0x8013F6C | 1;
SwapTurnOrder = 0x8014CA4 | 1;
TurnValuesCleanUp = 0x8015330 | 1;
SpecialStatusesClear = 0x8015450 | 1;
CheckFocusPunch_ClearVarsBeforeTurnStarts = 0x80154A0 | 1;
HandleEndTurn_FinishBattle = 0x8015910 | 1;
GetBattleBank = 0x8016E24 | 1;
PressurePPLose = 0x8016EC8 | 1;
PressurePPLoseOnUsingImprision = 0x8016F90 | 1;
MarkBufferBankForExecution = 0x8017248 | 1;
CancelMultiTurnMoves = 0x80172E8 | 1;
WasUnableToUseMove = 0x8017354 | 1;
ResetSentPokesToOpponentValue = 0x80173D0 | 1;
sub_8017434 = 0x8017434 | 1;
sub_80174B8 = 0x80174B8 | 1;
BattleScriptPush = 0x8017520 | 1;
BattleScriptPushCursor = 0x8017544 | 1;
BattleScriptPop = 0x801756C | 1;
IsImprisoned = 0x8017A64 | 1;
/*HasNoMonsToSwitch = 0x8019C10 | 1;*/
AbilityBattleEffects = 0x8019F18 | 1;
BattleScriptExecute = 0x801BBE4 | 1;
BattleScriptPushCursorAndCallback = 0x801BC24 | 1;
/*ItemBattleEffects = 0x801BC68 | 1;*/
JumpIfMoveFailed = 0x801DB10 | 1;
CheckWonderGuardAndLevitate = 0x801EAB8 | 1;
SetMoveEffect = 0x801FF20 | 1;
MoveValuesCleanUp = 0x8022598 | 1;
IsTwoTurnsMove = 0x802A1C0 | 1;
AttacksThisTurn = 0x802A224 | 1;
SetControllerToPlayer = 0x802E314 | 1;
PlayerBufferExecCompleted = 0x802E33C | 1;
/*HandleInputChooseTarget = 0x802E674 | 1;*/
HandleMoveSwitching = 0x802EF58 | 1;
sub_802F730 = 0x802F730 | 1;
sub_802F768 = 0x802F768 | 1;
CompleteOnInactiveTextPrinter = 0x802FE94 | 1;
Task_LaunchLvlUpAnim = 0x8030238 | 1;
DestroyExpTaskAndCompleteOnInactiveTextPrinter = 0x8030350 | 1;
MoveSelectionDisplayMoveNames = 0x8030830 | 1;
MoveSelectionDisplayPpNumber = 0x80308CC | 1;
/*MoveSelectionDisplayMoveType = 0x8030940 | 1;*/
MoveSelectionCreateCursorAt = 0x80309CC | 1;
MoveSelectionDestroyCursorAt = 0x8030A14 | 1;
sub_8033AC8 = 0x8033AC8 | 1;
sub_8033EEC = 0x8033EEC | 1;
DecompressTrainerFrontPic = 0x80346C4 | 1;
DecompressTrainerBackPic = 0x8034750 | 1;
load_gfxc_health_bar = 0x8034964 | 1;
HandleLowHpMusicChange = 0x8035244 | 1;
BattleStopLowHpSound = 0x8035314 | 1;
SetControllerToOpponent = 0x8035A7D | 1;
CompleteOnBattlerSpriteCallbackDummy = 0x8035AE8 | 1;
CompleteOnBankSpriteCallbackDummy2 = 0x8035B20 | 1;
sub_8035BE8 = 0x8035BE8 | 1;
OpponentBufferExecCompleted = 0x8036530 | 1;
ShouldSwitchIfPerishSong = 0x8039188 | 1;
AreStatsRaised = 0x80397E0 | 1;
/*AI_TrySwitchOrUseItem = 0x8039C84 | 1;*/
ModulateByTypeEffectiveness = 0x8039DB4 | 1;
ShouldUseItem = 0x803A1F4 | 1;
CountAliveMons = 0x803F5B4 | 1;
GetDefaultMoveTarget = 0x803F6A0 | 1; /*REWRITE!************************************************/
SetMultiuseSpriteTemplateToTrainerBack = 0x803F864 | 1;
GetSecretBaseTrainerPicIndex = 0x8040EEC | 1;
GetSecretBaseTrainerNameIndex = 0x8040F2C | 1;
AdjustFriendship = 0x80436F8 | 1;
sub_80447F0 = 0x80447F0 | 1;
GetLinkTrainerFlankId = 0x8043620 | 1;
GetBattlerMultiplayerId = 0x8043658 | 1;
SetBattleBarStruct = 0x8048150 | 1;
SetHealthboxSpriteVisible = 0x80481D4 | 1;
MoveBattleBar = 0x8049FD8 | 1;
SpriteCB_ReleaseMonFromBall = 0x804B268 | 1;
IsCryPlayingOrClearCrySongs = 0x80721F0;
DestroyAnimSprite = 0x8072740 | 1;
StoreSpriteCallbackInData6 = 0x8074A68 | 1;
sub_8074E14 = 0x8074E14 | 1;
GetBattlerSide = 0x80751C4 | 1;
GetBattlerPosition = 0x80751D8 | 1;
GetBattlerAtPosition = 0x80751E8 | 1;
IsDoubleBattle = 0x8075290 | 1;
InitSpriteDataForLinearTranslation = 0x80754B8 | 1;
GetBattlerSpriteSubpriority = 0x807685C | 1;
StartTheBattle = 0x807F868 | 1;
GetTrainerALoseText = 0x80806EC | 1;
GetTrainerAWinText = 0x8080710 | 1;
BattleIntroSlide2 = 0x80BC6C8 | 1;
BattleIntroSlide3 = 0x80BC9D4 | 1;
BattleIntroSlideLink = 0x80BCC4C | 1;
/*BattleAI_SetupAIData = 0x80C6DA0 | 1;*/
BattleAI_ChooseMoveOrAction = 0x80C6F44 | 1;
RecordAbilityBattle = 0x80C71D0 | 1;
RecordItemBattle = 0x80C7208 | 1;
StrCpyDecodeToDisplayedStringBattle = 0x80D77F4 | 1;
TryGetStatusString = 0x80D7804 | 1;
ExpandBattleTextBuffPlaceholders = 0x80D8418 | 1;
ChooseMoveUsedParticle = 0x80D86C8 | 1;
ChooseTypeOfMoveUsedString = 0x80D8720 | 1;
BattlePutTextOnWindow = 0x80D87BC | 1;
SetPpNumbersPaletteInMoveSelection = 0x80D89DC | 1;
/*GetBattleTowerTrainerPicIndex = 0x80E6078 | 1; //Ruby Leftover*/
/*GetBattleTowerTrainerClassId = 0x80E60E4 | 1; //Ruby Leftover*/
/*GetBattleTowerTrainerName = 0x80E6150 | 1; //Ruby Leftover*/
GetEreaderTrainerFrontSpriteId = 0x80E7420 | 1;
GetEreaderTrainerClassId = 0x80E7440 | 1;
GetEreaderTrainerName = 0x80E7460 | 1;
sub_80EB2E0 = 0x80EB2E0 | 1;
/*IsLinkDoubleBattle = 0x811FA20 | 1;*/
sub_8127EC4 = 0x8127EC4 | 1;
sub_8127FF4 = 0x8127FF4 | 1;
GetTrainerTowerTrainerClassId = 0x815DA10 | 1;
GetTrainerTowerTrainerName = 0x815DA28 | 1;
GetTrainerTowerTrainerPicIndex = 0x815DA3C | 1;
CopyTrainerTowerPlayerLostText = 0x815DBF4 | 1;
CopyTrainerTowerPlayerWonText = 0x815DC40 | 1;

/*Poke Ball Functions*/
SpriteCB_TestBallThrow = 0x804ABD4 | 1;
PokeBallOpenParticleAnimation = 0x80F06A4 | 1;
GreatBallOpenParticleAnimation = 0x80F0C28 | 1;
SafariBallOpenParticleAnimation = 0x80F0A20 | 1;
UltraBallOpenParticleAnimation = 0x80F0B20 | 1;
MasterBallOpenParticleAnimation = 0x80F0F10 | 1;
DiveBallOpenParticleAnimation = 0x80F091C | 1;
RepeatBallOpenParticleAnimation = 0x80F0DB4 | 1;
TimerBallOpenParticleAnimation = 0x80F0818 | 1;
PremierBallOpenParticleAnimation = 0x80F103C | 1;

/*Pokemon Functions*/
EmitSetMonData = 0x800DFF0 | 1;
ZeroPlayerPartyMons = 0x0803DA14 |1;
ZeroEnemyPartyMons = 0x803DA34 | 1;
CreateMon = 0x803DA54 | 1;
CreateBoxMon = 0x803DAC4 | 1;
CalculateMonStats = 0x0803E47C | 1;
GetMonGender = 0x803F720 | 1;
GetBoxMonGender = 0x803F730 | 1;
GetGenderFromSpeciesAndPersonality = 0x803F78C | 1;
GetMonData = 0x0803FBE8|1;
SetMonData = 0x0804037C|1;
CopyMon = 0x8040B08 | 1;
SendMonToPC = 0x8040B90 | 1;
GetPartyAbility = 0x8040D38 | 1;
CalculatePlayerPartyCount = 0x8040C3C | 1;
CalculatePPWithBonus = 0x804101C | 1;
SpeciesToNationalPokedexNum = 0x8043298 | 1; 
MonGainEVs = 0x8043890 | 1;
GetPokeFlavourRelation = 0x8044234 | 1;
IsTradedMon = 0x804425C | 1;
IsOtherTrainer = 0x8044288 | 1;
MonRestorePP = 0x80442E4 | 1;
GetSetPokedexFlag = 0x8044898 | 1;
GetPokedexHeightWeight = 0x8088E38 | 1;
ReducePartyToThree = 0x80A03D8 | 1;
pokemon_order_func = 0x8128030 | 1;

/*Item Functions*/
IsMail = 0x80980F8 | 1;
CopyItemName = 0x8099E90 | 1;
CheckBagHasItem = 0x8099F40 | 1;
SanitizeItemId = 0x809A8A4 | 1;
ItemId_GetHoldEffect = 0x809A924 | 1;
ItemId_GetHoldEffectParam = 0x809A948 | 1;
ItemId_GetMystery2 = 0x809A9B4 | 1;
GetPocketByItemId = 0x809A9D8 | 1;
ItemId_GetType = 0x809A9FC | 1;
AddMoney = 0x809FDA0 | 1;

/*Overworld Functions*/
GetTrainerEncounterMusicId = 0x804367C | 1;
Overworld_GetMapTypeOfSaveblockLocation = 0x8056188 | 1;
GetCurrentMapBattleScene = 0x8056288 | 1;
MapGridGetMetatileBehaviorAt = 0x8058F78 | 1;
MetatileBehavior_IsSandOrDeepSand = 0x8059B74 | 1;
MetatileBehavior_IsSurfableWaterOrUnderwater = 0x8059CA4 | 1;
MetatileBehavior_IsTallGrass = 0x8059F34 | 1;
MetatileBehavior_IsLongGrass = 0x8059F4C | 1;
MetatileBehavior_IsIndoorEncounter = 0x8059F88 | 1;
MetatileBehavior_IsMountain = 0x8059F9C | 1;
MetatileBehavior_IsDeepOrOceanWater = 0x805A01C | 1;
PlayerGetDestCoords = 0x805C538 | 1;
TestPlayerAvatarFlags = 0x805C74C | 1;
GetEventObjectScriptPointerByEventObjectId = 0x805FC38 | 1;
GetRunningDirectionAnimNum = 0x8063520 | 1;
npc_apply_anim_looping = 0x8063554 | 1;
npc_apply_direction = 0x8064678 | 1;
npc_obj_ministep_stop_on_arrival = 0x8064788 | 1;
ScriptContext2_Enable = 0x8069940 | 1;
ScriptContext1_SetupScript = 0x8069AE4 | 1;
ScriptContext1_Stop = 0x8069B28 | 1;
PlayNewMapMusic = 0x8071A74 | 1;

/*InitTrainerBattleVariables = 0x8080110 | 1;*/
TrainerBattleLoadArgs = 0x8080168 | 1;
SetMapVarsToTrainer = 0x80801F0 | 1;
TrainerBattleLoadArg16 = 0x80800A8 | 1;
TrainerBattleLoadArg8 = 0x80800B4 | 1;
/*BattleSetup_ConfigureTrainerBattle = 0x8080228 | 1;*/
TrainerWantsBattle = 0x8080334 | 1;
/*GetTrainerFlagFromScriptPointer = 0x8080378 | 1;*/
CB2_EndTrainerBattle = 0x80804AC | 1;
TrainerCanApproachPlayer = 0x8081BEC | 1;
TrainerApproachPlayer = 0x8081E68 | 1;
GetRematchTrainerId = 0x810CE64 | 1;

/*Quest Log*/
QuestLogRemtachBattleStore = 0x811231C | 1;
IsQuestLogActive = 0x8111C2C | 1;

/*Party Menu Functions*/
sub_811FA78 = 0x811FA78 | 1;
sub_812029C = 0x812029C | 1;
sub_8122D78 = 0x8122D78 | 1;

/*Palette Functions*/
LoadCompressedPalette = 0x80703A8 | 1;
LoadPalette = 0x80703EC | 1;
FillPalette = 0x870424 | 1;
TransferPlttBuffer = 0x8070474 | 1;
UpdatePaletteFade = 0x80704D0 | 1;
ResetPaletteFade = 0x8070528 | 1;
ReadPlttIntoBuffers = 0x8070548 | 1;
BeginNormalPaletteFade = 0x8070588 | 1;
ResetPaletteStructByUid = 0x8070A0C | 1;
ResetPaletteStruct = 0x8070A28 | 1;
ResetPaletteFadeControl = 0x8070A84 | 1;
GetPaletteNumByUid = 0x8070B58 | 1;
UpdateNormalPaletteFade = 0x8070B8C | 1;
InvertPlttBuffer = 0x8070CCC | 1;
TintPlttBuffer = 0x8070D14 | 1;
UnfadePlttBuffer = 0x8070DE8 | 1;
BeginFastPaletteFade = 0x8070E44 | 1;
BeginFastPaletteFadeInternal = 0x8070E68 | 1;
UpdateFastPaletteFade = 0x8070EEC | 1;
BeginHardwarePaletteFade = 0x8071264 | 1;
UpdateHardwarePaletteFade = 0x8071300 | 1;
UpdateBlendRegisters = 0x807141C | 1;
IsSoftwarePaletteFadeFinishing = 0x8071470 | 1;
BlendPalettes = 0x80714D4 | 1;
BlendPalettesUnfaded = 0x8071510 | 1;
TintPalette_GrayScale = 0x8071544 | 1;
TintPalette_GrayScale2 = 0x8071594 | 1;
TintPalette_SepiaTone = 0x80715F4 | 1;
TintPalette_CustomTone = 0x8071660 | 1;


/*Other Functions*/
Malloc = 0x8002B9C | 1;
Calloc = 0x8002BB0 | 1;
Free = 0x8002BC4 | 1;
CreateSprite = 0x8006F8C | 1;
SpriteCallbackDummy = 0x800760C | 1;
FreeSpriteTilesByTag = 0x800874C | 1;
FreeSpritePaletteByTag = 0x800874C | 1;
GetSpriteTileStartByTag = 0x8008804 | 1;
IndexOfSpritePaletteTag = 0x80089E8 | 1;
StringCopy = 0x8008D84 | 1;
ConvertIntToDecimalStringN = 0x8008E78 | 1;
LZDecompressVram = 0x800EBC0 | 1;
LoadCompressedSpriteSheetUsingHeap = 0x800F034 | 1;
LoadCompressedSpritePaletteUsingHeap = 0x800F078 | 1;
nullsub_8 = 0x802E310 | 1;
CountTrailingZeroBits = 0x8045190 | 1;
GetEventObjectIdByLocalIdAndMapInternal = 0x805DFF4 | 1;
GetEventObjectIdByLocalId = 0x805E044 | 1;
FlagSet = 0x806E680 | 1;
FlagClear = 0x806E6A8 | 1;
FlagGet = 0x806E6D0 | 1;
VarGet = 0x806E568 | 1;
VarSet = 0x806E584 | 1;
Random = 0x8044EC8 | 1;
LoadCompressedPalette = 0x80703A8 | 1;
PlayBGM = 0x80722A0 | 1;
PlaySE = 0x80722CC | 1;
ResetTasks = 0x80773BC | 1;
CreateTask = 0x807741C | 1;
InsertTask = 0x8077470 | 1;
DestroyTask = 0x8077508 | 1;
RunTasks = 0x8077578 | 1;
FindFirstActiveTask = 0x80775A8 | 1;
TaskDummy = 0x80775E4 | 1;
SetTaskFuncWithFollowupFunc = 0x80775E8 | 1;
SwitchTaskToFollowupFunc = 0x807761C | 1;
FuncIsActiveTask = 0x8077650 | 1;
FindTaskIdByFunc = 0x8077688 | 1;
GetTaskCount = 0x80776B8 | 1;
ReturnEmptyStringIfNull = 0x80806C8 | 1;
m4aSongNumStop = 0x81DD1C0 | 1;
m4aMPlayContinue = 0x81DD26C | 1;
m4aMPlayVolumeControl = 0x81DE0E0 | 1;
CpuSet = 0x81E3B64 | 1;
Sqrt = 0x81E3B9C | 1;
udivsi = 0x81E460C | 1;
umodsi = 0x81E4684 | 1;

/*The Deal Breaker*/
break_func = 0x08416304 | 1;
