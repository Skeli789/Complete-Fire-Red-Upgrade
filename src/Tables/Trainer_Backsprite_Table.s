.thumb
.text
.align 2

.global gTrainerBackPicPaletteTable
.global gTrainerBackAnimsPtrTable
.global gTrainerBackPicCoords
.global TrainerBackspritesTable

.equ BackspriteOAM, 0x824F018
.equ BackspriteRotscaleTable, 0x82348C8
.equ BackspriteAnimCmd, 0x80120C4 | 1

.equ BackspriteImageTable_Red, 0x8234718
.equ BackspriteImageTable_Green, 0x8234740
@.equ BackspriteImageTable_Brendan, 0x82347A8
@.equ BackspriteImageTable_May, 0x82347C8
.equ BackspriteImageTable_Primo, 0x8234768
.equ BackspriteImageTable_OldMan, 0x8234788

gTrainerBackPicPaletteTable:
.word 0x8E76EBC, 0x0 @Red
.word 0x8E76EE4, 0x1 @Green
.word Brendan_Back_0Pal, 0x2 @0x8E550A4, 0x2 @Brendan
.word May_Back_0Pal, 0x3 @0x8E553CC, 0x3 @May
.word 0x8E76F0C, 0x4 @Primo
.word 0x8E76F34, 0x5 @Old Man
.word Marlon_Back_0Pal, 0x6

gTrainerBackAnimsPtrTable:
.word 0x8239F44 @Red
.word 0x8239F4C @Green
.word 0x8239F64 @Brendan
.word 0x8239F6C @May
.word 0x8239F54 @Primo
.word 0x8239F5C @Old Man
.word 0x8239F6C @Marlon

gTrainerBackPicCoords:
.byte 0x8, 0x5, 0x0, 0x0 @Red
.byte 0x8, 0x5, 0x0, 0x0 @Green
.byte 0x8, 0x4, 0x0, 0x0 @Brendan
.byte 0x8, 0x4, 0x0, 0x0 @May
.byte 0x8, 0x4, 0x0, 0x0 @Primo
.byte 0x8, 0x4, 0x0, 0x0 @Old Man
.byte 0x8, 0x4, 0x0, 0x0 @Marlon

TrainerBackspritesTable:
@0 - Red
.hword 0xFFFF
.hword 0x0
.word BackspriteOAM, 0x0
.word BackspriteImageTable_Red
.word BackspriteRotscaleTable
.word BackspriteAnimCmd

@1 - Green
.hword 0xFFFF
.hword 0x0
.word BackspriteOAM, 0x0
.word BackspriteImageTable_Green
.word BackspriteRotscaleTable
.word BackspriteAnimCmd

@2 - Brendan
.hword 0xFFFF
.hword 0x0
.word BackspriteOAM, 0x0
.word BackspriteImageTable_Brendan
.word BackspriteRotscaleTable
.word BackspriteAnimCmd

@3 - May
.hword 0xFFFF
.hword 0x0
.word BackspriteOAM, 0x0
.word BackspriteImageTable_May
.word BackspriteRotscaleTable
.word BackspriteAnimCmd

@4 - Primo (Poke Dude)
.hword 0xFFFF
.hword 0x0
.word BackspriteOAM, 0x0
.word BackspriteImageTable_Primo
.word BackspriteRotscaleTable
.word BackspriteAnimCmd

@5 - Old Man
.hword 0xFFFF
.hword 0x0
.word BackspriteOAM, 0x0
.word BackspriteImageTable_OldMan
.word BackspriteRotscaleTable
.word BackspriteAnimCmd

@6 - Marlon (From Pokemon Unbound, feel free to remove)
.hword 0xFFFF
.hword 0x0
.word BackspriteOAM, 0x0
.word BackspriteImageTable_Marlon
.word BackspriteRotscaleTable
.word BackspriteAnimCmd

BackspriteImageTable_Brendan:
.word Brendan_Back_1Tiles
.hword 0x800, 0x0

.word Brendan_Back_2Tiles
.hword 0x800, 0x0

.word Brendan_Back_3Tiles
.hword 0x800, 0x0

.word Brendan_Back_0Tiles
.hword 0x800, 0x0

BackspriteImageTable_May:
.word May_Back_1Tiles
.hword 0x800, 0x0

.word May_Back_2Tiles
.hword 0x800, 0x0

.word May_Back_3Tiles
.hword 0x800, 0x0

.word May_Back_0Tiles
.hword 0x800, 0x0

BackspriteImageTable_Marlon:
.word Marlon_Back_1Tiles
.hword 0x800, 0x0

.word Marlon_Back_2Tiles
.hword 0x800, 0x0

.word Marlon_Back_2Tiles
.hword 0x800, 0x0

.word Marlon_Back_0Tiles
.hword 0x800, 0x0
