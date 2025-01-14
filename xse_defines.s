.thumb

.equ ShowTrainerIntroSpeech, 0x34
.equ ShowTrainerCantBattleSpeech, 0x35
.equ GetTrainerFlag, 0x36
.equ EndTrainerApproach, 0x37
.equ SetUpTrainerEncounterMusic, 0x38
.equ IsTrainerReadyForRematch, 0x3A
.equ HasEnoughMonsForDoubleBattle, 0x3D
.equ CAMERA_START, 0x113
.equ CAMERA_END, 0x114
.equ SetUpTrainerMovement, 0x13A

.equ CAMERA, 0x7F
.equ FOLLOWER, 0xFE
.equ PLAYER, 0xFF
.equ PLAYERFACING, 0x800C
.equ LASTRESULT, 0x800D
.equ CHOSEN_ITEM, 0x800E
.equ LASTTALKED, 0x800F

.equ look_down, 0x0
.equ look_up, 0x1
.equ look_left, 0x2
.equ look_right, 0x3

.equ walk_down_very_slow, 0x8
.equ walk_up_very_slow, 0x9
.equ walk_left_very_slow, 0xA 
.equ walk_right_very_slow, 0xB
.equ walk_down_slow, 0xC
.equ walk_up_slow, 0xD
.equ walk_left_slow, 0xE 
.equ walk_right_slow, 0xF
.equ walk_down, 0x10
.equ walk_up, 0x11
.equ walk_left, 0x12
.equ walk_right, 0x13
.equ jump_2_down, 0x14
.equ jump_2_up, 0x15
.equ jump_2_left, 0x16
.equ jump_2_right, 0x17
.equ pause_xshort, 0x18
.equ pause_vshort, 0x19
.equ pause_short, 0x1A
.equ pause_long, 0x1C
.equ run_down, 0x1D
.equ run_up, 0x1E
.equ run_left, 0x1F
.equ run_right, 0x20
.equ walk_down_onspot, 0x21
.equ walk_up_onspot, 0x22
.equ walk_left_onspot, 0x23
.equ walk_right_onspot, 0x24
.equ walk_down_onspot_fast, 0x25
.equ walk_up_onspot_fast, 0x26
.equ walk_left_onspot_fast, 0x27
.equ walk_right_onspot_fast, 0x28
.equ walk_down_onspot_vfast, 0x29
.equ walk_up_onspot_vfast, 0x2A
.equ walk_left_onspot_vfast, 0x2B
.equ walk_right_onspot_vfast, 0x2C
.equ walk_down_onspot_fastest, 0x2D
.equ walk_up_onspot_fastest, 0x2E
.equ walk_left_onspot_fastest, 0x2F
.equ walk_right_onspot_fastest, 0x30
.equ slide_down_slow, 0x31
.equ slide_up_slow, 0x32
.equ slide_left_slow, 0x33
.equ slide_right_slow, 0x34
.equ slide_down, 0x35
.equ slide_up, 0x36
.equ slide_left, 0x37
.equ slide_right, 0x38
.equ slide_down_fast, 0x39
.equ slide_up_fast, 0x3A
.equ slide_left_fast, 0x3B
.equ slide_right_fast, 0x3C
.equ player_run_down, 0x3D
.equ player_run_up, 0x3E
.equ player_run_left, 0x3F
.equ player_run_right, 0x40
.equ player_run_down_slow, 0x41
.equ player_run_up_slow, 0x42
.equ player_run_left_slow, 0x43
.equ player_run_right_slow, 0x44
.equ start_anim_in_direction, 0x45
.equ jump_special_down, 0x46
.equ jump_special_up, 0x47
.equ jump_special_left, 0x48
.equ jump_special_right, 0x49
.equ face_player, 0x4A
.equ face_away_player, 0x4B
.equ lock_facing, 0x4C
.equ unlock_facing, 0x4D
.equ jump_down, 0x4E
.equ jump_up, 0x4F
.equ jump_left, 0x50
.equ jump_right, 0x51
.equ jump_onspot_down, 0x52
.equ jump_onspot_up, 0x53
.equ jump_onspot_left, 0x54
.equ jump_onspot_right, 0x55
.equ jump_onspot_down_up, 0x56
.equ jump_onspot_up_down, 0x57
.equ jump_onspot_left_right, 0x58
.equ jump_onspot_right_left, 0x59
.equ face_default, 0x5A
.equ nurse_bow, 0x5B
.equ enable_jump_landing_ground_effect, 0x5C
.equ disable_jump_landing_ground_effect, 0x5D
.equ disable_anim, 0x5E
.equ enable_anim, 0x5F
.equ set_invisible, 0x60
.equ set_visible, 0x61
.equ exclaim, 0x62
.equ say_question, 0x63
.equ say_cross, 0x64
.equ say_double_exclaim, 0x65
.equ say_smile, 0x66
.equ reveal_trainer, 0x67
.equ smash_rock, 0x68
.equ cut_tree, 0x69
.equ set_fixed_priority, 0x6A
.equ clear_fixed_priority, 0x6B
.equ init_affine_anim, 0x6C
.equ clear_affine_anim, 0x6D
.equ end_m, 0xFE

.equ EventScript_RevealTrainer, 0x81A4FBA
.equ EventScript_NoDoubleTrainerBattle, 0x81A4F20
.equ EventScript_NotEnoughMonsForDoubleBattle, 0x81A4F19

.equ VAR_8006_LOC, 0x20370C4
.equ SELECTED_EVENT_OBJECT, 0x3005074

.equ BLUE, 0x0
.equ RED, 0x1
.equ BLACK, 0x3

.equ NO, 0x0
.equ YES, 0x1

.equ FALSE, 0x0
.equ TRUE, 0x1

.equ NOT_SET, 0x0
.equ SET, 0x1

.equ BOY, 0x0
.equ GIRL, 0x1

.equ DOWN, 0x1
.equ UP, 0x2
.equ LEFT, 0x3
.equ RIGHT, 0x4

.equ PARTY_SIZE, 0x6
.equ MAX_MON_MOVES, 0x4
.equ NUM_STATS, 0x6
.equ NUM_NATURES, 25

.equ FADEIN_BLACK, 0x0
.equ FADEOUT_BLACK, 0x1
.equ FADEIN_WHITE, 0x2
.equ FADEOUT_WHITE, 0x3

.equ ENDMART, 0x0

.equ TWO_MULTICHOICE_OPTIONS, 0x20
.equ THREE_MULTICHOICE_OPTIONS, 0x21
.equ FOUR_MULTICHOICE_OPTIONS, 0x22
.equ FIVE_MULTICHOICE_OPTIONS, 0x23
.equ SIX_MULTICHOICE_OPTIONS, 0x24
.equ SEVEN_MULTICHOICE_OPTIONS, 0x25

.equ SCROLL_MULTICHOICE_NUM, 0x8000
.equ SCROLL_MULTICHOICE_HEIGHT, 0x8001

.equ TYPE_NORMAL, 0x0
.equ TYPE_FIGHTING, 0x1
.equ TYPE_FLYING, 0x2
.equ TYPE_POISON, 0x3
.equ TYPE_GROUND, 0x4
.equ TYPE_ROCK, 0x5
.equ TYPE_BUG, 0x6
.equ TYPE_GHOST, 0x7
.equ TYPE_STEEL, 0x8
.equ TYPE_TYPELESS, 0x9
.equ TYPE_FIRE, 0xA
.equ TYPE_WATER, 0xB
.equ TYPE_GRASS, 0xC
.equ TYPE_ELECTRIC, 0xD
.equ TYPE_PSYCHIC, 0xE
.equ TYPE_ICE, 0xF
.equ TYPE_DRAGON, 0x10
.equ TYPE_DARK, 0x11
.equ TYPE_ROOSTLESS, 0x13
.equ TYPE_FAIRY, 0x17

.equ NUM_HIDDEN_POWER_TYPES, 16

.equ DELAY_HALFSECOND, 0x20
.equ DELAY_1SECOND, 0x40

.equ SUNDAY, 0x0
.equ MONDAY, 0x1
.equ TUESDAY, 0x2
.equ WEDNESDAY, 0x3
.equ THURSDAY, 0x4
.equ FRIDAY, 0x5
.equ SATURDAY, 0x6

.equ MAP_SCRIPT_TERMIN, 0
.equ MAP_SCRIPT_ON_LOAD, 1						@Setmaptile
.equ MAP_SCRIPT_ON_FRAME_TABLE, 2				@Level Script
.equ MAP_SCRIPT_ON_TRANSITION, 3				@On entering map/not on menu close
.equ MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, 4		@Level Script 2 (eg. face up on enter map)
.equ MAP_SCRIPT_ON_RESUME, 5					@On entering map/on menu close
.equ MAP_SCRIPT_ON_DIVE_WARP, 6
.equ MAP_SCRIPT_ON_RETURN_TO_FIELD, 7
.equ LEVEL_SCRIPT_TERMIN, 0
