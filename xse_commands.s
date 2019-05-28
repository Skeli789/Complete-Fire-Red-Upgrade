@All scripting macros off of XSE and http://sphericalice.com/romhacking/documents/script/index.html

@Message box types
.equ MSG_OBTAIN, 0x0
.equ MSG_FIND, 0x1
.equ MSG_FACE, 0x2
.equ MSG_SIGN, 0x3
.equ MSG_KEEPOPEN, 0x4
.equ MSG_YESNO, 0x5
.equ MSG_NORMAL, 0x6
.equ MSG_POKENAV, 0xA

@Compare values
.equ lessthan, 0x0 @A is less than B
.equ equal, 0x1 @A is equal to B
.equ greaterthan, 0x2 @A is more than B
.equ lessorequal, 0x3 @A is less than or equal to B
.equ greaterorequal, 0x4 @A is greater than or equal to B
.equ notequal, 0x5 @A is not equal to B
.equ false, 0x0
.equ true, 0x1

@azuril13's if command becuase it's nice
.equ _goto,	0x06
.equ _jump, 0x06
.equ _call,	0x07

.macro if condition goto_or_call script
.byte \goto_or_call, \condition
.word \script
.endm

@Macro macros, from XSE.
.macro msgbox msgbox_pointer msgbox_type
.byte 0xF
.byte 0x0
.word \msgbox_pointer
.byte 0x9
.byte \msgbox_type
.endm

.macro giveitem giveitem_item giveitem_amount giveitem_msgtype
.byte 0x1A
.hword 0x8000
.hword \giveitem_item
.byte 0x1A
.hword 0x8001
.hword \giveitem_amount
.byte 0x9
.byte \giveitem_msgtype
.endm

.macro giveitem2 giveitem2_item giveitem2_amount giveitem2_song
.byte 0x1A
.hword 0x8000
.hword \giveitem2_item
.byte 0x1A
.hword 0x8001
.hword \giveitem2_amount
.byte 0x1A
.hword 0x8002
.hword \giveitem2_song
.byte 0x9
.byte 0x9
.endm

.macro giveitem3 giveitem3_decoration
.byte 0x1A
.word 0x8000
.hword \giveitem3_decoration
.byte 0x9
.byte 0x7
.endm

.macro wildbattle wildbattle_species wildbattle_level wildbattle_item
.byte 0xB6
.hword \wildbattle_species
.byte \wildbattle_level
.hword \wildbattle_item
.byte 0xB7
.endm

.macro registernav registernav_trainer
.byte 0x1A
.hword 0x8000
.hword \registernav_trainer
.byte 0x9
.byte 0x8
.endm

.macro multichoiceoption text num
.byte 0x16 @setvar
.2byte 0x8006
.2byte \num
.byte 0xF, 0 @loadpointer
.4byte \text
.byte 0x25 @special
.2byte 0x25
.endm

@Commands
.macro nop
.byte 0x0
.endm

.macro nop1
.byte 0x1
.endm

.macro end
.byte 0x2
.endm

.macro return
.byte 0x3
.endm

.macro call call_destination
.byte 0x4
.word \call_destination
.endm

.macro goto goto_destination
.byte 0x5
.word \goto_destination
.endm

.macro jump goto_destination
.byte 0x5
.word \goto_destination
.endm

.macro if1 if1_condition if1_goto_destination
.byte 0x6
.byte \if1_condition
.word \if1_goto_destination
.endm

.macro if2 if2_condition if2_call_destination
.byte 0x7
.byte \if2_condition
.word \if2_call_destination
.endm

.macro gotostd gotostd_param1
.byte 0x8
.byte \gotostd_param1
.endm

.macro callstd callstd_param1
.byte 0x9
.byte \callstd_param1
.endm

.macro gotostdif gotostdif_cond gotostdif_param1
.byte 0xA
.byte \gotostdif_cond
.byte \gotostdif_param1
.endm

.macro callstdif callstdif_cond callstdif_param1
.byte 0xB
.byte \callstdif_cond
.byte \callstdif_param1
.endm

@jumpram's documentation may not be complete
.macro jumpram
.byte 0xC
.endm

.macro killscript
.byte 0xD
.endm

.macro setbyte setbyte_value
.byte 0xE
.byte \setbyte_value
.endm

.macro loadpointer loadpointer_bank loadpointer_pointer
.byte 0xF
.byte \loadpointer_bank
.word \loadpointer_pointer
.endm

.macro setbyte2 setbyte2_bank setbyte2_value
.byte 0x10
.byte \setbyte2_bank
.byte \setbyte2_value
.endm

.macro writebytetooffset wbto_byte wbto_pointer
.byte 0x11
.byte \wbto_byte
.word \wbto_pointer
.endm

.macro loadbytefrompointer lbfp_bank_dest lbfp_pointer
.byte 0x12
.byte \lbfp_bank_dest
.word \lbfp_pointer
.endm

.macro setfarbyte setfarbyte_bank setfarbyte_pointer
.byte 0x13
.byte \setfarbyte_bank
.word \setfarbyte_pointer
.endm

.macro copyscriptbanks copyscriptbanks_bank_destination copyscriptbanks_bank_source
.byte 0x14
.byte \copyscriptbanks_bank_destination
.byte \copyscriptbanks_bank_source
.endm

.macro copybyte copybyte_dest copybyte_source
.byte 0x15
.word \copybyte_dest
.word \copybyte_source
.endm

.macro setvar setvar_var setvar_value
.byte 0x16
.hword \setvar_var
.hword \setvar_value
.endm

.macro addvar addvar_var addvar_value
.byte 0x17
.hword \addvar_var
.hword \addvar_value
.endm

.macro subvar subvar_var subvar_value
.byte 0x18
.hword \subvar_var
.hword \subvar_value
.endm

.macro copyvar copyvar_destination copyvar_source
.byte 0x19
.hword \copyvar_destination
.hword \copyvar_source
.endm

.macro copyvarifnotzero copyvarifnotzero_var copyvarifnotzero_source
.byte 0x1A
.hword \copyvarifnotzero_var
.hword \copyvarifnotzero_source
.endm

.macro comparebanks comparebanks_a comparebanks_b
.byte 0x1B
.byte \comparebanks_a
.byte \comparebanks_b
.endm

.macro comparebanktobyte comparebanktobyte_bank comparebanktobyte_byte
.byte 0x1C
.byte \comparebanktobyte_bank
.byte \comparebanktobyte_byte
.endm

.macro comparebanktofarbyte comparebbanktofarbyte_bank comparebanktofarbyte_pointer
.byte 0x1D
.byte \comparebanktofarbyte_bank
.word \comparebanktofarbyte_pointer
.endm

.macro comparefarbytetobank comparefarbytetobank_pointer comparefarbytetobank_bank
.byte 0x1E
.word \comparefarbytetobank_pointer
.byte \comparefarbytetobank_bank
.endm

.macro comparefarbytetobyte comparefarbytetobyte_pointer comparefarbytetobyte_byte
.byte 0x1F
.word \comparefarbytetobyte_pointer
.byte \comparefarbytetobyte_byte
.endm

.macro comparefarbytes comparefarbytes_param1 comparefarbytes_param2
.byte 0x20
.word \comparefarbytes_param1
.word \comparefarbytes_param2
.endm

.macro compare compare_var compare_value
.byte 0x21
.hword \compare_var
.hword \compare_value
.endm

.macro comparevars comparevars_var1 comparevars_var2
.byte 0x22
.hword \comparevars_var1
.hword \comparevars_var2
.endm

.macro callasm callasm_pointer
.byte 0x23
.word \callasm_pointer
.endm

.macro cmd24 cmd24_pointer
.byte 0x24
.word \cmd24_pointer
.endm

.macro special special_param
.byte 0x25
.hword \special_param
.endm

.macro special2 special2_outputvar special2_function
.byte 0x26
.hword \special2_outputvar
.hword \special2_function
.endm

.macro waitstate
.byte 0x27
.endm

.macro pause pause_time
.byte 0x28
.hword \pause_time
.endm

.macro setflag setflag_flag
.byte 0x29
.hword \setflag_flag
.endm

.macro clearflag clearflag_flag
.byte 0x2A
.hword \clearflag_flag
.endm

.macro checkflag checkflag_flag
.byte 0x2B
.hword \checkflag_flag
.endm

@nop in FR
.macro cmd2C
.byte 0x2C
.endm

@nop in FR, no other info right now.
.macro checkdailyflags
.byte 0x2D
.endm

.macro resetvars gettime @ (stores hours in 0x8000, minutes in 0x8001 and seconds on 0x8002)
.byte 0x2E
.endm

.macro sound sound_id
.byte 0x2F
.hword \sound_id
.endm

.macro checksound
.byte 0x30
.endm

.macro fanfare fanfare_id
.byte 0x31
.hword \fanfare_id
.endm

.macro waitfanfare
.byte 0x32
.endm

.macro playsong playsong_songid playsong_unknown
.byte 0x33
.hword \playsong_songid
.byte \playsong_unknown
.endm

.macro playsong2 playsong2_song
.byte 0x34
.hword \playsong2_song
.endm

.macro fadedefault
.byte 0x35
.endm

.macro fadesong fadesong_song
.byte 0x36
.hword \fadesong_song
.endm

.macro fadeout fadeout_speed
.byte 0x37
.byte \fadeout_speed
.endm

.macro fadein fadein_speed
.byte 0x38
.byte \fadein_speed
.endm

.macro warp warp_bank warp_map warp_warp warp_x_axis warp_y_axis
.byte 0x39
.byte \warp_bank
.byte \warp_map
.byte \warp_warp
.hword \warp_x_axis
.hword \warp_y_axis
.endm

.macro warpmuted warpmuted_bank warpmuted_map warpmuted_warp warpmuted_x_axis warpmuted_y_axis
.byte 0x3A
.byte \warpmuted_bank
.byte \warpmuted_map
.byte \warpmuted_warp
.hword \warpmuted_x_axis
.hword \warpmuted_y_axis
.endm

.macro warpwalk warpwalk_bank warpwalk_map warpwalk_warp warpwalk_x_axis warpwalk_y_axis
.byte 0x3B
.byte \warpwalk_bank
.byte \warpwalk_map
.byte \warpwalk_warp
.hword \warpwalk_x_axis
.hword \warpwalk_y_axis
.endm

.macro warphole warphole_bank warphole_map
.byte 0x3C
.byte \warphole_bank
.byte \warphole_map
.endm

.macro warpteleport warpteleport_bank warpteleport_map warpteleport_warp warpteleport_x_axis warpteleport_y_axis
.byte 0x3D
.byte \warpteleport_bank
.byte \warpteleport_map
.byte \warpteleport_warp
.hword \warpteleport_x_axis
.hword \warpteleport_y_axis
.endm

.macro warp3 warp3_bank warp3_map warp3_warp warp3_x_axis warp3_y_axis
.byte 0x3E
.byte \warp3_bank
.byte \warp3_map
.byte \warp3_warp
.hword \warp3_x_axis
.hword \warp3_y_axis
.endm

.macro setwarpplace setwarpplace_bank setwarpplace_map setwarpplace_warp setwarpplace_x_axis setwarpplace_y_axis
.byte 0x3F
.byte \setwarpplace_bank
.byte \setwarpplace_map
.byte \setwarpplace_warp
.hword \setwarpplace_x_axis
.hword \setwarpplace_y_axis
.endm

.macro warp4 warp4_bank warp4_map warp4_warp warp4_x_axis warp4_y_axis
.byte 0x40
.byte \warp4_bank
.byte \warp4_map
.byte \warp4_warp
.hword \warp4_x_axis
.hword \warp4_y_axis
.endm

.macro warp5 warp5_bank warp5_map warp5_warp warp5_x_axis warp5_y_axis
.byte 0x41
.byte \warp5_bank
.byte \warp5_map
.byte \warp5_warp
.hword \warp5_x_axis
.hword \warp5_y_axis
.endm

.macro getplayerpos getplayerpos_x_var getplayerpos_y_var
.byte 0x42
.hword \getplayerpos_x_var
.hword \getplayerpos_y_var
.endm

.macro countpokemon
.byte 0x43
.endm

.macro additem additem_index additem_quantity
.byte 0x44
.hword \additem_index
.hword \additem_quantity
.endm

.macro removeitem removeitem_index removeitem_quantity
.byte 0x45
.hword \removeitem_index
.hword \removeitem_quantity
.endm

.macro checkitemroom checkitemroom_index checkitemroom_quantity
.byte 0x46
.hword \checkitemroom_index
.hword \checkitemroom_quantity
.endm

.macro checkitem checkitem_index checkitem_quantity
.byte 0x47
.hword \checkitem_index
.hword \checkitem_quantity
.endm

.macro checkitemtype checkitemtype_index
.byte 0x48
.hword \checkitemtype_index
.endm

.macro addpcitem addpcitem_index addpcitem_quantity
.byte 0x49
.hword \addpcitem_index
.hword \addpcitem_quantity
.endm

.macro checkpcitem checkpcitem_index checkpcitem_quantity
.byte 0x4A
.hword \checkpcitem_index
.hword \checkpcitem_quantity
.endm

.macro adddecoration adddecoration_decoration
.byte 0x4B
.hword \adddecoration_decoration
.endm

.macro removedecoration removedecoration_decoration
.byte 0x4C
.hword \removedecoration_decoration
.endm

.macro testdecoration testdecoration_decoration
.byte 0x4D
.hword \testdecoration_decoration
.endm

.macro checkdecoration checkdecoration_decoration
.byte 0x4E
.hword \checkdecoration_decoration
.endm

.macro applymovement applymovement_person_id applymovement_pointer
.byte 0x4F
.hword \applymovement_person_id
.word \applymovement_pointer
.endm

.macro applymovementpos applymovementpos_var applymovementpos_pointer
.byte 0x50
.hword \applymovementpos_var
.word \applymovementpos_pointer
.endm

.macro waitmovement waitmovement_index
.byte 0x51
.hword \waitmovement_index
.endm

.macro waitmovementpos waitmovementpos_index waitmovementpos_x waitmovementpos_y
.byte 0x52
.hword \waitmovementpos_index
.byte \waitmovementpos_x
.byte \waitmovementpos_y
.endm

.macro hidesprite hidesprite_param
.byte 0x53
.hword \hidesprite_param
.endm

.macro hidespritepos hidespritepos_index hidespritepos_x hidespritepos_y
.byte 0x54
.hword \hidespritepos_index
.byte \hidespritepos_x
.byte \hidespritepos_y
.endm

.macro showsprite showsprite_id
.byte 0x55
.hword \showsprite_id
.endm

.macro movesprite movesprite_person movesprite_x movesprite_y
.byte 0x57
.hword \movesprite_person
.hword \movesprite_x
.hword \movesprite_y
.endm

.macro faceplayer
.byte 0x5A
.endm

.macro trainerbattle0 trainerbattle0_type trainerbattle0_index trainerbattle0_filler trainerbattle0_intro trainerbattle0_loss
.byte 0x5C
.byte 0x0
.hword \trainerbattle0_index
.hword \trainerbattle0_filler
.word \trainerbattle0_intro
.word \trainerbattle0_loss
.endm

.macro trainerbattle1 trainerbattle1_type trainerbattle1_index trainerbattle1_filler trainerbattle1_intro trainerbattle1_loss trainerbattle1_extra
.byte 0x5C
.byte 0x1
.hword \trainerbattle1_index
.hword \trainerbattle1_filler
.word \trainerbattle1_intro
.word \trainerbattle1_loss
.word \trainerbattle1_extra
.endm

.macro trainerbattle2 trainerbattle2_type trainerbattle2_index trainerbattle2_filler trainerbattle2_intro trainerbattle2_loss trainerbattle2_extra
.byte 0x5C
.byte 0x2
.hword \trainerbattle2_index
.hword \trainerbattle2_filler
.word \trainerbattle2_intro
.word \trainerbattle2_loss
.word \trainerbattle2_extra
.endm

.macro trainerbattle3 trainerbattle3_type trainerbattle3_index trainerbattle3_filler trainerbattle3_intro
.byte 0x5C
.byte 0x3
.hword \trainerbattle3_index
.hword \trainerbattle3_filler
.word \trainerbattle3_intro
.endm

.macro trainerbattle4 trainerbattle4_type trainerbattle4_index trainerbattle4_filler trainerbattle4_intro trainerbattle4_loss trainerbattle4_extra
.byte 0x5C
.byte 0x4
.hword \trainerbattle4_index
.hword \trainerbattle4_filler
.word \trainerbattle4_intro
.word \trainerbattle4_loss
.word \trainerbattle4_extra
.endm

.macro trainerbattle5 trainerbattle5_type trainerbattle5_index trainerbattle5_filler trainerbattle5_intro trainerbattle5_loss
.byte 0x5C
.byte 0x5
.hword \trainerbattle5_index
.hword \trainerbattle5_filler
.word \trainerbattle5_intro
.word \trainerbattle5_loss
.endm

.macro trainerbattle6 trainerbattle6_type trainerbattle6_index trainerbattle6_filler trainerbattle6_intro trainerbattle6_loss trainerbattle6_extra trainerbattle6_extra2
.byte 0x5C
.byte 0x6
.hword \trainerbattle6_index
.hword \trainerbattle6_filler
.word \trainerbattle6_intro
.word \trainerbattle6_loss
.word \trainerbattle6_extra
.word \trainerbattle6_extra2
.endm

.macro trainerbattle7 trainerbattle7_type trainerbattle7_index trainerbattle7_filler trainerbattle7_intro trainerbattle7_loss trainerbattle7_extra
.byte 0x5C
.byte 0x7
.hword \trainerbattle7_index
.hword \trainerbattle7_filler
.word \trainerbattle7_intro
.word \trainerbattle7_loss
.word \trainerbattle7_extra
.endm

.macro trainerbattle8 trainerbattle8_type trainerbattle8_index trainerbattle8_filler trainerbattle8_intro trainerbattle8_loss trainerbattle8_extra trainerbattle8_extra2
.byte 0x5C
.byte 0x8
.hword \trainerbattle8_index
.hword \trainerbattle8_filler
.word \trainerbattle8_intro
.word \trainerbattle8_loss
.word \trainerbattle8_extra
.word \trainerbattle8_extra2
.endm

.macro trainerbattle9 trainerbattle9_type trainerbattle9_index trainerbattle9_filler trainerbattle9_intro trainerbattle9_loss
.byte 0x5C
.byte 0x9
.hword \trainerbattle9_index
.hword \trainerbattle9_filler
.word \trainerbattle9_intro
.word \trainerbattle9_loss
.endm

@trainerbattle 0xA FOE_1_ID FOE_2_ID PARTNER_ID PARTNER_BACKSPRITE_ID 0x0 DEFEAT_TEXT_A DEFEAT_TEXT_B
.macro trainerbattle10 trainerbattle10_type trainerbattle10_foeindex1 trainerbattle10_foeindex2 trainerbattle10_partnerindex trainerbattle10_partnerbackspriteindex trainerbattle10_filler trainerbattle10_loss1 trainerbattle10_loss2
.byte 0x5C
.byte 0xA
.hword \trainerbattle10_foeindex1
.hword \trainerbattle10_foeindex2
.hword \trainerbattle10_partnerindex
.hword \trainerbattle10_partnerbackspriteindex
.hword \trainerbattle10_filler
.word \trainerbattle10_loss1
.word \trainerbattle10_loss2
.endm

@trainerbattle 0xB FOE_1_ID FOE_2_ID FOE_1_NPC_ID FOE_2_NPC_ID 0x0 INTRO_TEXT_A INTRO_TEXT_B DEFEAT_TEXT_A DEFEAT_TEXT_B CANNOT_BATTLE_TEXT_A CANNOT_BATTLE_TEXT_B
.macro trainerbattle11 trainerbattle11_type trainerbattle11_foeindex1 trainerbattle11_foeindex2 trainerbattle11_foenpcid1 trainerbattle11_foenpcid2 trainerbattle11_filler trainerbattle11_intro1 trainerbattle11_intro2 trainerbattle11_loss1 trainerbattle11_loss2 trainerbattle11_cannotbattle1 trainerbattle11_cannotbattle2
.byte 0x5C
.byte 0xB
.hword \trainerbattle11_foeindex1
.hword \trainerbattle11_foeindex2
.byte \trainerbattle11_foenpcid1
.byte \trainerbattle11_foenpcid2
.hword \trainerbattle11_filler
.word \trainerbattle11_intro1
.word \trainerbattle11_intro2
.word \trainerbattle11_loss1
.word \trainerbattle11_loss2
.word \trainerbattle11_cannotbattle1
.word \trainerbattle11_cannotbattle2
.endm

@trainerbattle 0xC FOE_ID PARTNER_ID PARTNER_BACKSPRITE_ID 0x0 DEFEAT_TEXT_A
.macro trainerbattle12 trainerbattle12_type trainerbattle12_foeindex trainerbattle12_partnerindex trainerbattle12_partnerbackspriteindex trainerbattle12_filler trainerbattle12_loss
.byte 0x5C
.byte 0xC
.hword \trainerbattle12_foeindex
.hword \trainerbattle12_partnerindex
.hword \trainerbattle12_partnerbackspriteindex
.hword \trainerbattle12_filler
.word \trainerbattle12_loss
.endm

.macro repeattrainerbattle
.byte 0x5D
.endm

.macro endtrainerbattle
.byte 0x5E
.endm

.macro endtrainerbattle2
.byte 0x5F
.endm

.macro checktrainerflag checktrainerflag_trainer
.byte 0x60
.hword \checktrainerflag_trainer
.endm

@XSE had it wrong, i'm correcting them
.macro settrainerflag settrainerflag_trainer
.byte 0x61
.hword \settrainerflag_trainer
.endm

.macro cleartrainerflag cleartrainerflag_trainer
.byte 0x62
.hword \cleartrainerflag_trainer
.endm

.macro movesprite2 movesprite2_person movesprite2_x movesprite2_y
.byte 0x63
.hword \movesprite2_person
.hword \movesprite2_x
.hword \movesprite2_y
.endm

.macro moveoffscreen moveoffscreen_person
.byte 0x64
.hword \moveoffscreen_person
.endm

.macro spritebehave spritebehave_person spritebehave_behaviour
.byte 0x65
.hword \spritebehave_person
.byte \spritebehave_behavior
.endm

.macro waitmsg
.byte 0x66
.endm

.macro preparemsg preparemsg_text
.byte 0x67
.word \preparemsg_text
.endm

.macro closeonkeypress
.byte 0x68
.endm

.macro lockall
.byte 0x69
.endm

.macro lock
.byte 0x6A
.endm

.macro releaseall
.byte 0x6B
.endm

.macro release
.byte 0x6C
.endm

.macro waitkeypress
.byte 0x6D
.endm

.macro yesnobox yesnobox_x yesnobox_y
.byte 0x6E
.byte \yesnobox_x
.byte \yesnobox_y
.endm

.macro multichoice multichoice_x multichoice_y multichoice_list multichoice_b_enabled
.byte 0x6F
.byte \multichoice_x
.byte \multichoice_y
.byte \multichoice_list
.byte \multichoice_b_enabled
.endm

.macro multichoice2 multichoice2_x multichoice2_y multichoice2_list multichoice2_default_pos multichoice2_b_enabled
.byte 0x70
.byte \multichoice2_x
.byte \multichoice2_y
.byte \multichoice2_list
.byte \multichoice2_default_pos
.byte \multichoice2_b_enabled
.endm

.macro multichoice3 multichoice3_x multichoice3_y multichoice3_list multichoice3_item_per_row multichoice3_b_enabled
.byte 0x71
.byte \multichoice3_x
.byte \multichoice3_y
.byte \multichoice3_list
.byte \multichoice3_items_per_row
.byte \multichoice3_b_enabled
.endm

.macro showbox showbox_left_pos showbox_top_pos showbox_width showbox_height
.byte 0x72
.byte \showbox_left_pos
.byte \showbox_top_pos
.byte \showbox_width
.byte \showbox_height
.endm

.macro hidebox hidebox_left_pos hidebox_top_pos hidebox_width hidebox_height
.byte 0x73
.byte \hidebox_left_pos
.byte \hidebox_top_pos
.byte \hidebox_width
.byte \hidebox_height
.endm

.macro clearbox clearbox_left_pos clearbox_top_pos clearbox_width clearbox_height
.byte 0x74
.byte \clearbox_left_pos
.byte \clearbox_top_pos
.byte \clearbox_width
.byte \clearbox_height
.endm

.macro showpokepic showpokepic_species showpokepic_x showpokepic_y
.byte 0x75
.hword \showpokepic_species
.byte \showpokepic_x
.byte \showpokepic_y
.endm

.macro hidepokepic
.byte 0x76
.endm

.macro showcontestwinner showcontestwinner_byte
.byte 0x77
.byte \showcontestwinner_byte
.endm

.macro braille braille_text
.byte 0x78
.word \braille_text
.endm

.macro givepokemon givepokemon_species givepokemon_level givepokemon_item givepokemon_unknown1 givepokemon_unknown2 givepokemon_unknown3
.byte 0x79
.hword \givepokemon_species
.byte \givepokemon_level
.hword \givepokemon_item
.word \givepokemon_unknown1
.word \givepokemon_unknown2
.byte \givepokemon_unknown3
.endm

.macro giveegg giveegg_species
.byte 0x7A
.hword \giveegg_species
.endm

.macro setpkmnpp setpkmnpp_party_slot setpkmnpp_attk_slot setpkmnpp_amount setpkmnmove @(u8 poke_slot, u8 move_slot, u16 new_move)
.byte 0x7B
.byte \setpkmnpp_party_slot
.byte \setpkmnpp_attk_slot
.byte \setpkmnpp_new_attk
.endm

.macro checkattack checkattack_attack
.byte 0x7C
.hword \checkattack_attack
.endm

.macro bufferpokemon bufferpokemon_buffer bufferpokemon_species
.byte 0x7D
.byte \bufferpokemon_buffer
.hword \bufferpokemon_species
.endm

.macro bufferfirstpokemon bufferfirstpokemon_buffer
.byte 0x7E
.byte \bufferfirstpokemon_buffer
.endm

.macro bufferpartypokemon bufferpartypokemon_buffer bufferpartypokemon_slot
.byte 0x7F
.byte \bufferpartypokemon_buffer
.hword \bufferpartypokemon_slot
.endm

.macro bufferitem bufferitem_buffer bufferitem_item
.byte 0x80
.byte \bufferitem_buffer
.hword \bufferitem_item
.endm

.macro bufferdecoration bufferdecoration_buffer bufferdecoration_decoration
.byte 0x81
.byte \bufferdecoration_buffer
.hword \bufferdecoration_decoration
.endm

.macro bufferattack bufferattack_buffer bufferattack_attack
.byte 0x82
.byte \bufferattack_buffer
.hword \bufferattack_attack
.endm

.macro buffernumber buffernumber_buffer buffernumber_number
.byte 0x83
.byte \buffernumber_buffer
.hword \buffernumber_number
.endm

.macro bufferstd bufferstd_buffer bufferstd_string
.byte 0x84
.byte \bufferstd_buffer
.hword \bufferstd_string
.endm

.macro bufferstring bufferstring_buffer bufferstring_pointer
.byte 0x85
.byte \bufferstring_buffer
.hword \bufferstring_pointer
.endm

.macro pokemart pokemart_pointer
.byte 0x86
.word \pokemart_pointer
.endm

.macro pokemart2 pokemart2_pointer
.byte 0x87
.word \pokemart2_pointer
.endm

.macro pokemart3 pokemart3_pointer
.byte 0x88
.word \pokemart3_pointer
.endm

.macro pokecasino pokecasino_param1
.byte 0x89
.hword \pokecasino_param1
.endm

.macro cmd8A createplantedberry @(u8 plantID, u8 berryID, u8 berryState)
.byte 0x8A
.byte \plantID
.byte \berryID
.byte \berryState
.endm

.macro choosecontestpokemon
.byte 0x8B
.endm

.macro startcontest
.byte 0x8C
.endm

.macro showcontestresults
.byte 0x8D
.endm

.macro contestlinktransfer
.byte 0x8E
.endm

.macro random random_limit
.byte 0x8F
.hword \random_limit
.endm

.macro givemoney givemoney_value givemoney_byte
.byte 0x90
.word \givemoney_value
.byte \givemoney_byte
.endm

.macro paymoney paymoney_value paymoney_byte
.byte 0x91
.word \paymoney_value
.byte \paymoney_byte
.endm

.macro checkmoney checkmoney_value checkmoney_byte
.byte 0x92
.word \checkmoney_value
.byte \checkmoney_byte
.endm

.macro showmoney showmoney_x showmoney_y
.byte 0x93
.byte \showmoney_x
.byte \showmoney_y
.endm

.macro hidemoney hidemoney_x hidemoney_y
.byte 0x94
.byte \hidemoney_x
.byte \hidemoney_y
.endm

.macro updatemoney updatemoney_x updatemoney_y
.byte 0x95
.byte \updatemoney_x
.byte \updatemoney_y
.endm

.macro cmd96 checkpokenewsfor @(u16 reportID(may be in a var)) Stores return value in 0x800D. Pokenews stands for those things you sometimes see on tv.
.byte 0x96
.hword \reportID
.endm

.macro fadescreen fadescreen_effect
.byte 0x97
.byte \fadescreen_effect
.endm

.macro fadescreendelay fadescreendelay_effect fadescreendelay_time
.byte 0x98
.byte \fadescreendelay_effect
.byte \fadescreendelay_time
.endm

.macro darken darken_size
.byte 0x99
.hword \darken_size
.endm

.macro lighten lighten_size
.byte 0x9A
.byte \lighten_size
.endm

.macro preparmsg2 preparemsg2_pointer
.byte 0x9B
.word \preparemsg2_pointer
.endm

.macro doanimation doanimation_param
.byte 0x9C
.hword \doanimation_param
.endm

.macro setanimation setanimation_anim setanimation_slot
.byte 0x9D
.byte \setanimation_anim
.hword \setanimation_slot
.endm

.macro checkanimation checkanimation_param
.byte 0x9E
.hword \checkanimation_param
.endm

.macro sethealingplace sethealingplace_param
.byte 0x9F
.hword \sethealingplace_param
.endm

.macro checkgender
.byte 0xA0
.endm

.macro cry cry_species cry_effect
.byte 0xA1
.hword \cry_species
.hword \cry_effect
.endm

.macro setmaptile setmaptile_x setmaptile_y setmaptile_tile setmaptile_attrib
.byte 0xA2
.hword \setmaptile_x
.hword \setmaptile_y
.hword \setmaptile_tile
.hword \setmaptile_attrib
.endm

.macro resetweather
.byte 0xA3
.endm

.macro setweather setweather_type
.byte 0xA4
.hword \setweather_type
.endm

.macro doweather
.byte 0xA5
.endm

.macro cmdA6 cmdA6_byte activate_per_step_task @(u8 caseID), table of functions that get called everytime you take a step, think stepping on ash grass.
.byte 0xA6
.byte \caseID
.endm

.macro setmapfooter setmapfooter_param
.byte 0xA7
.hword \setmapfooter_param
.endm

.macro spritelevelup spritelevelup_person spritelevelup_bank spritelevelup_map spritelevelup_unknown
.byte 0xA8
.hword \spritelevelup_person
.byte \spritelevelup_bank
.byte \spritelevelup_map
.byte \spritelevelup_unknown
.endm

.macro restorespritelevel restorespritelevel_person restorespritelevel_bank restorespritelevel_map
.byte 0xA9
.hword \restorespritelevel_person
.byte \restorespritelevel_bank
.byte \restorespritelevel_map
.endm

.macro createsprite createsprite_sprite createsprite_person createsprite_x createsprite_y createsprite_behave createsprite_face
.byte 0xAA
.byte \createsprite_sprite
.byte \createsprite_person
.hword \createsprite_x
.hword \createsprite_y
.byte \createsprite_behave
.byte \createsprite_face
.endm

.macro spriteface2 spriteface2_person spriteface2_face
.byte 0xAB
.byte \spriteface2_person
.byte \spriteface2_face
.endm

.macro setdooropened setdooropened_x setdooropened_y
.byte 0xAC
.hword \setdooropened_x
.hword \setdooropened_y
.endm

.macro setdoorclosed setdoorclosed_x setdoorclosed_y
.byte 0xAD
.hword \setdoorclosed_x
.hword \setdoorclosed_y
.endm

.macro doorchange
.byte 0xAE
.endm

.macro setdooropened2 setdooropened2_x setdooropened2_y
.byte 0xAF
.hword \setdooropened2_x
.hword \setdooropened2_y
.endm

.macro setdoorclosed2 setdoorclosed2_x setdoorclosed2_y
.byte 0xB0
.hword \setdoorclosed2_x
.hword \setdoorclosed2_y
.endm

.macro cmdB1
.byte 0xB1
.endm

.macro cmdB2 nopB2 @does nothing, just returns 0
.byte 0xB2
.endm

.macro checkcoins checkcoins_var
.byte 0xB3
.hword \checkcoins_var
.endm

.macro givecoins givecoins_param
.byte 0xB4
.hword \givecoins_param
.endm

.macro removecoins removecoins_param
.byte 0xB5
.hword \removecoins_param
.endm

.macro setwildbattle setwildbattle_species setwildbattle_level setwildbattle_item
.byte 0xB6
.hword \setwildbattle_species
.byte \setwildbattle_level
.hword \setwildbattle_item
.endm

.macro dowildbattle
.byte 0xB7
.endm

.macro setvirtualaddress setvirtualaddress_param
.byte 0xB8
.word \setvirtualaddress_param
.endm

.macro virtualgoto virtualgoto_pointer
.byte 0xB9
.word \virtualgoto_pointer
.endm

.macro virtualcall virtualcall_pointer
.byte 0xBA
.word \virtualcall_pointer
.endm

.macro virtualgotoif virtualgotoif_condition virtualgotoif_pointer
.byte 0xBB
.byte \virtualgotoif_condition
.word \virtualgotoif_pointer
.endm

.macro virtualcallif virtualcallif_condition virtualcallif_pointer
.byte 0xBC
.byte \virtualcallif_condition
.word \virtualcallif_pointer
.endm

.macro virtualmsgbox virtualmsgbox_text
.byte 0xBD
.word \virtualmsgbox_text
.endm

.macro virtualloadpointer virtualloadpointer_pointer
.byte 0xBE
.word \virtualloadpointer_pointer
.endm

.macro virtualbuffer virtualbuffer_buffer virtualbuffer_pointer
.byte 0xBF
.byte \virtualbuffer_buffer
.word \virtualbuffer_pointer
.endm

.macro showcoins showcoins_x showcoins_y
.byte 0xC0
.byte \showcoins_x
.byte \showcoins_y
.endm

.macro hidecoins hidecoins_x hidecoins_y
.byte 0xC1
.byte \hidecoins_x
.byte \hidecoins_y
.endm

.macro updatecoins updatecoins_x updatecoins_y
.byte 0xC2
.byte \updatecoins_x
.byte \updatecoins_y
.endm

.macro cmdC3 cmdC3_hidden_var increment_encrypted_counter @(u8 counterID)
.byte 0xC3
.byte \counterID
.endm

.macro warp6 warp6_bank warp6_map warp6_warp warp6_x warp6_y
.byte 0xC4
.byte \warp6_bank
.byte \warp6_map
.byte \warp6_warp
.hword \warp6_x
.hword \warp6_y
.endm

.macro waitcry
.byte 0xC5
.endm

.macro bufferboxname bufferboxname_buffer bufferboxname_box
.byte 0xC6
.byte \bufferboxname_buffer
.hword \bufferboxname_box
.endm

.macro textcolor textcolor_byte nopC7
.byte 0xC7
.byte \textcolor_byte
.endm

.macro cmdC8 cmdC8_param
.byte 0xC8
.word \cmdC8_param
.endm

.macro cmdC9
.byte 0xC9
.endm

.macro signmsg
.byte 0xCA
.endm

.macro msgboxsign
.byte 0xCA
.endm

.macro normalmsg
.byte 0xCB
.endm

.macro msgboxnormal
.byte 0xCB
.endm

.macro comparehiddenvar comparehiddenvar_byte comparehiddenvar_value nopCC
.byte 0xCC
@.byte \comparehiddenvar_byte Uncomment if FR.
@.word \comparehiddenvar_value
.endm

.macro setobedience setobedience_slot
.byte 0xCD
.hword \setobedience_slot
.endm

.macro checkobedience checkobedience_slot
.byte 0xCE
.hword \checkobedience_slot
.endm

.macro executeram
.byte 0xCF
.endm

.macro setworldmapflag setworldmapflag_param nopD0
.byte 0xD0
@.hword \setworldmapflag_param uncomment if FR.
.endm

.macro warpteleport2
.byte 0xD1
.endm

.macro setcatchlocation setcatchlocation_slot setcatchlocation_map
.byte 0xD2
.hword \setcatchlocation_slot
.byte \setcatchlocation_map
.endm

.macro braille2 braille2_text
.byte 0xD3
.word \braille2_text
.endm

.macro bufferitems bufferitems_buffer bufferitems_item bufferitems_amount
.byte \bufferitems_buffer
.hword \bufferitems_item
.hword \bufferitems_amount
.endm

.macro cmdD5
.byte 0xD5
.endm

.macro cmdD6
.byte 0xD6
.endm

.macro warp7 warp7_bank warp7_map warp7_warp warp7_x warp7_y
.byte 0xD7
.byte \warp7_bank
.byte \warp7_map
.byte \warp7_warp
.hword \warp7_x
.hword \warp7_y
.endm

.macro cmdD8
.byte 0xD8
.endm

.macro cmdD9
.byte 0xD9
.endm

.macro hidebox2
.byte 0xDA
.endm

.macro preparemsg3 preparemsg3_pointer
.byte 0xDB
.word \preparemsg3_pointer
.endm

.macro fadescreen3 fadescreen3_param
.byte 0xDC
.byte \fadescreen3_param
.endm

.macro buffertrainerclass buffertrainerclass_buffer buffertrainerclass_class
.byte 0xDD
.byte \buffertrainerclass_buffer
.hword \buffertrainerclass_class
.endm

.macro buffertrainername buffertrainername_buffer buffertrainername_trainer
.byte 0xDE
.byte \buffertrainername_buffer
.hword \buffertrainername_trainer
.endm

.macro pokenavcall pokenavcall_text
.byte 0xDF
.word \pokenavcall_text
.endm

.macro warp8 warp8_bank warp8_map warp8_warp warp8_x warp8_y
.byte 0xE0
.byte \warp8_bank
.byte \warp8_map
.byte \warp8_warp
.hword \warp8_x
.hword \warp8_y
.endm

.macro buffercontesttype buffercontesttype_param1 buffercontesttype_param2
.byte 0xE1
.byte \buffercontesttype_param1
.hword \buffercontesttype_param2
.endm

.macro bufferitems2 bufferitems2_buffer bufferitems2_item bufferitems2_amount
.byte 0xE2
.byte \bufferitems2_buffer
.hword \bufferitems2_item
.hword \bufferitems2_amount
.endm
