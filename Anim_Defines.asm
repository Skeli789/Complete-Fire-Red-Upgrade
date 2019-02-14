.text
.equ rom, 0x8000000

@banks
.equ bank_attacker, 0x0
.equ bank_target, 0x1
.equ attacker_partner, 0x2
.equ target_partner, 0x3

@sides for commands A, B
.equ side_attacker, 0x2
.equ side_target, 0x3

@background IDs
.equ BG_DARK, 0x1
.equ BG_GHOST, 0x2
.equ BG_PSYCHIC, 0x3
.equ BG_IMPACT_ON_OPPONENT, 0x4
.equ BG_IMPACT_ON_PLAYER, 0x5
.equ BG_IMPACT_CONTESTS, 0x6
.equ BG_DRILL_BATTLE, 0x7
.equ BG_DRILL_CONTESTS, 0x8
.equ BG_HIGHSPEED_ON_OPPONENT, 0x9
.equ BG_HIGHSPEED_ON_PLAYER, 0xA
.equ BG_THUNDER, 0xB
.equ BG_GUILLOTINE_ON_OPPONENT, 0xC
.equ BG_GUILLOTINE_ON_PLAYER, 0xD
.equ BG_GUILLOTINE_CONTESTS, 0xE
.equ BG_ICE, 0xF
.equ BG_COSMIC, 0x10
.equ BG_SEISMICTOSS_SKUUPPERCUT, 0x11
.equ BG_FLYING_BATTLE, 0x12
.equ BG_FLYING_CONTESTS, 0x13
.equ BG_AURORABEAM, 0x14
.equ BG_FISSURE, 0x15
.equ BG_BUG_ON_OPPONENT, 0x16
.equ BG_BUG_ON_PLAYER, 0x17
.equ BG_SOLARBEAM_ON_OPPONENT, 0x18
.equ BG_SOLARBEAM_ON_PLAYER, 0x19
.equ BG_SOLARBEAM_CONTESTS, 0x1A
.equ BG_ROCK_WRECKER, 0x1B
.equ BG_WATER, 0x1C
.equ BG_NIGHTMARE, 0x1D
.equ BG_LEAF_STORM, 0x1E
.equ BG_FIRE, 0x1F
.equ BG_FIRE_2, 0x20
.equ BG_GIGA_IMPACT_ON_OPPONENT, 0x21
.equ BG_GIGA_IMPACT_ON_PLAYER, 0x22
.equ BG_WATER_2, 0x23
.equ BG_POISON, 0x24
.equ BG_MOON, 0x25
.equ BG_HIGH_SPEED, 0x26
.equ BG_ELECTRIC_TERRAIN, 0x27
.equ BG_GRASSY_TERRAIN, 0x28
.equ BG_MISTY_TERRAIN, 0x29
.equ BG_PSYCHIC_TERRAIN, 0x2A
.equ BG_SPACIAL_REND_ON_OPPONENT, 0x2B
.equ BG_SPACIAL_REND_ON_PLAYER, 0x2C
.equ BG_DARK_VOID, 0x2D
.equ BG_FOCUS_BLAST, 0x2E
.equ BG_GUNK_SHOT, 0x2F
.equ BG_HYDRO_CANNON, 0x30
.equ BG_TRICK_ROOM, 0x31
.equ BG_WONDER_ROOM, 0x32
.equ BG_MAGIC_ROOM, 0x33
.equ BG_HYPERSPACE_FURY, 0x34
.equ BG_BOLT_STRIKE, 0x35
.equ BG_ZMOVE_ACTIVATE, 0x36
.equ BG_TECTONIC_RAGE, 0x37
.equ BG_BLUE_SKY_DAY, 0x38
.equ BG_BLUE_SKY_AFTERNOON, 0x39
.equ BG_BLUE_SKY_NIGHT, 0x3A
.equ BG_ZMOVE_MOUNTAIN, 0x3B
.equ BG_NEVERENDING_NIGHTMARE, 0x3C
.equ BG_WATER_PULSE, 0x3D
.equ BG_INFERNO_OVERDRIVE, 0x3E
.equ BG_BLOOM_DOOM, 0x3F
.equ BG_SHATTERED_PSYCHE, 0x40
.equ BG_TWINKLE_TACKLE, 0x41
.equ BG_BLACKHOLE_ECLIPSE, 0x42
.equ BG_SOULSTEALING_7STAR_STRIKE_STILL_RAW, 0x43
.equ BG_MALICIOUS_MOONSAULT, 0x44
.equ BG_CLANGOROUS_SOULBLAZE, 0x45

@pals for pal fading tasks
.equ PAL_BG, 0x1
.equ PAL_ATK, 0x2
.equ PAL_DEF, 0x4
.equ PAL_ATK_PARTNER, 0x8
.equ PAL_DEF_PARTNER, 0x10
.equ PAL_ALL, 0x1f
.equ PAL_ALL_BANKS, 0x780
.equ PAL_PLAYER1, 0x80
.equ PAL_PLAYER2, 0x100
.equ PAL_OPPONENT1, 0x200
.equ PAL_OPPONENT2, 0x400

@commands
.macro loadparticle loadparticleID
.byte 0x0
.hword \loadparticleID
.endm

.macro unloadparticle unloadparticleID
.byte 0x1
.hword \unloadparticleID
.endm

.macro launchtemplate launchtemplatePtr launchtemplatePriority launchtemplateArgsNo launchtemplatearg0 launchtemplatearg1 launchtemplatearg2 launchtemplatearg3 launchtemplatearg4 launchtemplatearg5 launchtemplatearg6 launchtemplatearg7 launchtemplatearg8
.byte 0x2
.word \launchtemplatePtr
.byte \launchtemplatePriority
.byte \launchtemplateArgsNo
.hword \launchtemplatearg0
.hword \launchtemplatearg1
.hword \launchtemplatearg2
.hword \launchtemplatearg3
.hword \launchtemplatearg4
.hword \launchtemplatearg5
.hword \launchtemplatearg6
.hword \launchtemplatearg7
.hword \launchtemplatearg8
.endm

.macro launchtask launchtaskPtr launchtaskPriority launchtaskArgsNo launchtaskarg0 launchtaskarg1 launchtaskarg2 launchtaskarg3 launchtaskarg4 launchtaskarg5 launchtaskarg6 launchtaskarg7 launchtaskarg8
.byte 0x3
.word \launchtaskPtr
.byte \launchtaskPriority
.byte \launchtaskArgsNo
.hword \launchtaskarg0
.hword \launchtaskarg1
.hword \launchtaskarg2
.hword \launchtaskarg3
.hword \launchtaskarg4
.hword \launchtaskarg5
.hword \launchtaskarg6
.hword \launchtaskarg7
.hword \launchtaskarg8
.endm

.macro pause pause_cmdToWait
.byte 0x4
.byte \pause_cmdToWait
.endm

.macro waitanimation
.byte 0x5
.endm

.macro endanimation
.byte 0x8
.endm

.macro playsound1 playsound1ID
.byte 0x9
.hword \playsound1ID
.endm

.macro pokespritetoBG animABank
.byte 0xA
.byte \animABank
.endm

.macro pokespritefromBG animBBank
.byte 0xB
.byte \animBBank
.endm

.macro setblends setblends_value
.byte 0xC
.hword \setblends_value
.endm

.macro resetblends
.byte 0xD
.endm

.macro call call_cmd_ptr
.byte 0xE
.word \call_cmd_ptr
.endm

.macro return
.byte 0xF
.endm

.macro setarg setargID setargValue
.byte 0x10
.byte \setargID
.hword \setargValue
.endm

.macro choosetwoturnanim choosetwoturnanim1 choosetwoturnanim2
.byte 0x11
.word \choosetwoturnanim1
.word \choosetwoturnanim2
.endm

.macro jumpifmoveturnEQ jumpifmoveturnEQturn jumpifmoveturnEQPTR
.byte 0x12
.byte \jumpifmoveturnEQturn
.word \jumpifmoveturnEQPTR
.endm

.macro goto goto_cmdPtr
.byte 0x13
.word \goto_cmdPtr
.endm

.macro loadBG1 loadBG1ID
.byte 0x14
.byte \loadBG1ID
.endm

.macro loaddefaultBG
.byte 0x15
.endm

.macro waitforBG
.byte 0x16
.endm

.macro waitfortransparentBG
.byte 0x17
.endm

.macro loadBG2 loadBG2ID
.byte 0x18
.byte \loadBG2ID
.endm

.macro playsound2 playsound2ID playsound2Unkown
.byte 0x19
.hword \playsound2ID
.byte \playsound2Unkown
.endm

.macro pancontrol anim1Aarg1
.byte 0x1A
.byte \anim1Aarg1
.endm

.macro playsoundpanchange anim1BsongID anim1BArg3 anim1BArg4 anim1BArg5 anim1BArg6
.byte 0x1B
.hword \anim1BsongID
.byte \anim1BArg3
.byte \anim1BArg4
.byte \anim1BArg5
.byte \anim1BArg6
.endm

.macro soundcomplex anim1Carg1 anim1Carg2 anim1Carg3 anim1Carg4
.byte 0x1C
.hword \anim1Carg1
.byte \anim1Carg2
.byte \anim1Carg3
.byte \anim1Carg4
.endm

.macro playsound3 anim1Darg1 anim1Darg2 anim1Darg3
.byte 0x1D
.hword \anim1Darg1
.byte \anim1Darg2
.byte \anim1Darg3
.endm

.macro set_BLDCNT set_BLDCNTvalue
.byte 0x1E
.hword \set_BLDCNTvalue
.endm

.macro launchsoundtask launchsoundtaskPtr launchsoundtaskArgsNo launchsoundtaskarg0 launchsoundtaskarg1 launchsoundtaskarg2 launchsoundtaskarg3 launchsoundtaskarg4 launchsoundtaskarg5 launchsoundtaskarg6 launchsoundtaskarg7 launchsoundtaskarg8
.byte 0x1F
.word \launchsoundtaskPtr
.byte \launchsoundtaskArgsNo
.hword \launchsoundtaskarg0
.hword \launchsoundtaskarg1
.hword \launchsoundtaskarg2
.hword \launchsoundtaskarg3
.hword \launchsoundtaskarg4
.hword \launchsoundtaskarg5
.hword \launchsoundtaskarg6
.hword \launchsoundtaskarg7
.hword \launchsoundtaskarg8
.endm

.macro waitforsound
.byte 0x20
.endm

.macro jumpifargmatches jumpifargmatchesargID jumpifargmatchesValue jumpifargmatchesPtr
.byte 0x21
.byte \jumpifargmatchesargID
.hword \jumpifargmatchesValue
.word \jumpifargmatchesPtr
.endm

.macro anim22 anim22bank
.byte 0x22
.byte \anim22bank
.endm

.macro anim23 anim23bank
.byte 0x23
.byte \anim23bank
.endm

.macro jumpifnotinbattle jumpifnotinbattlePTR
.byte 0x24
.word \jumpifnotinbattlePTR
.endm

.macro chooseBG chooseBG1 chooseBG2 chooseBG3
.byte 0x25
.byte \chooseBG1
.byte \chooseBG2
.byte \chooseBG3
.endm

.macro playsoundpanchange2 anim26H0 anim26B0 anim26B1 anim26B2 anim26B3
.byte 0x26
.hword \anim26H0
.byte \anim26B0
.byte \anim26B1
.byte \anim26B2
.byte \anim26B3
.endm

.macro playsoundpanchange3 anim27H0 anim27B0 anim27B1 anim27B2 anim27B3
.byte 0x27
.hword \anim27H0
.byte \anim27B0
.byte \anim27B1
.byte \anim27B2
.byte \anim27B3
.endm

.macro leftbankBG_over_partnerBG anim28Bank
.byte 0x28
.byte \anim28Bank
.endm

.macro bankBG_over_partnerBG
.byte 0x29
.endm

.macro leftopponentbankBG_over_partnerBG anim2Aarg
.byte 0x2A
.byte \anim2Aarg
.endm

.macro makebankinvisible makebankinvisibleBank
.byte 0x2B
.byte \makebankinvisibleBank
.endm

.macro makebankvisible makebankvisibleBank
.byte 0x2C
.byte \makebankvisibleBank
.endm

.macro stopmusic
.byte 0x2F
.endm

@objtemplate

.macro objtemplate objtemplatetiletag objtemplatepaltag objtemplateoam objtemplateimageanim objtemplategfx objtemplaterotscale objtemplatecallback
.hword \objtemplatetiletag
.hword \objtemplatepaltag
.word \objtemplateoam
.word \objtemplateimageanim
.word \objtemplategfx
.word \objtemplaterotscale
.word \objtemplatecallback
.endm

@launchtask

.equ AnimTask_move_bank, 0x80989F9
.equ AnimTask_move_bank_2, 0x8098B1D
.equ AnimTask_sprite_transform, 0x80DF9BD
.equ AnimTask_play_growling_cry, 0x80DD149
.equ AnimTask_pal_fade_particle, 0x80BAA21
.equ AnimTask_pal_fade, 0x80BA7F9
.equ AnimTask_pal_fade_complex, 0x80B9BDD
.equ AnimTask_screen_shake, 0x80B94B5
.equ AnimTask_prepare_moving_psychicBG, 0x80DE6F1
.equ AnimTask_arg7_is_target_player, 0x80DEAB5
.equ AnimTask_arg7_is_attacker_opponent, 0x80BB921
.equ AnimTask_arg7_is_in_contests, 0x80BC02D
.equ AnimTask_scroll_background, 0x80BB82D
.equ AnimTask_steel_sprite, 0x80B86ED
.equ AnimTask_surf_wave, 0x80AB38D
.equ AnimTask_splash_down, 0x80A8339
.equ AnimTask_create_haze, 0x80AFAE5
.equ AnimTask_SwaggerBulkUp, 0x80A84B5
.equ AnimTask_flash_anim, 0x80BB661

@launchtemplate

.equ Template_Hit, 0x83E7C08
.equ Template_Fist, 0x83E6710
.equ Template_Healing_Circle, 0x83E37A4
.equ Template_Pal_Fade, 0x83E7B24
.equ Template_Ring, 0x83E4094
.equ Template_SlideMonToOffset, 0x83d4e9c
.equ Template_SlideMonToOriginalPos, 0x83d4e84

@calls

.equ SET_PSYCHIC_BG, 0x81D59BB
.equ UNSET_SCROLLING_BG, 0x81D59C7
.equ BURN_CHANCE_ANIM, 0x81D0950
.equ FREEZE_CHANCE_ANIM, 0x81D540A
.equ FREEZE_CHANCE_ANIM_DOUBLES, 0x81D548E
.equ PARALYZE_CHANCE_ANIM, 0x81D58D4
.equ POISON_BUBBLES, 0x81D575B
.equ HEALING_STARS, 0x81D569E
.equ HEALING_ANIM, 0x81D56C9
.equ RECOVER_LOAD_PARTICLES, 0x81D170D
.equ ABSORB_ANIM, 0x81CEEEC
.equ WATER_DIVE_ATTACK, 0x81C8CA2
.equ BUFF_EFFECT, 0x81CB267

@supercommands

.macro unsetscrollingBG
loaddefaultBG
waitforBG
setarg 0x7 0xffff
waitfortransparentBG
.endm
