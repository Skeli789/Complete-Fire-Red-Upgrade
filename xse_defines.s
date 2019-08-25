.thumb

.equ ShowTrainerIntroSpeech, 0x34
.equ ShowTrainerCantBattleSpeech, 0x35
.equ GetTrainerFlag, 0x36
.equ EndTrainerApproach, 0x37
.equ SetUpTrainerEncounterMusic, 0x38
.equ HasEnoughMonsForDoubleBattle, 0x3D
.equ CAMERA_START, 0x113
.equ CAMERA_END, 0x114
.equ SetUpTrainerMovement, 0x13A

.equ CAMERA, 0x7F
.equ PLAYER, 0xFF
.equ PLAYERFACING, 0x800C
.equ LASTRESULT, 0x800D
.equ LASTTALKED, 0x800F

.equ look_up, 0x1
.equ look_left, 0x2
.equ look_right, 0x3
.equ look_down, 0x4
.equ walk_down_slow, 0xC
.equ walk_up_slow, 0xD
.equ walk_left_slow, 0xE 
.equ walk_right_slow, 0xF

.equ walk_down, 0x10
.equ walk_up, 0x11
.equ walk_left, 0x12
.equ walk_right, 0x13

.equ walk_down_onspot, 0x25
.equ walk_up_onspot, 0x26
.equ walk_left_onspot, 0x27
.equ walk_right_onspot, 0x28

.equ pause_xshort, 0x18
.equ pause_vshort, 0x19
.equ pause_short, 0x1A
.equ pause_long, 0x1C

.equ exclaim, 0x62
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

.equ FADEIN_BLACK, 0x0
.equ FADEOUT_BLACK, 0x1
.equ FADEIN_WHITE, 0x2
.equ FADEOUT_WHITE, 0x3

.equ TWO_MULTICHOICE_OPTIONS, 0x20
.equ THREE_MULTICHOICE_OPTIONS, 0x21
.equ FOUR_MULTICHOICE_OPTIONS, 0x22
.equ FIVE_MULTICHOICE_OPTIONS, 0x23
.equ SIX_MULTICHOICE_OPTIONS, 0x24
.equ SEVEN_MULTICHOICE_OPTIONS, 0x25

.equ SCROLL_MULTICHOICE_NUM, 0x8000
.equ SCROLL_MULTICHOICE_QUANTITY, 0x8001

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
