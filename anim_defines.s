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

@For launchtemplate
.equ TEMPLATE_ATTACKER, 0x0
.equ TEMPLATE_ABOVE, 0x20
.equ TEMPLATE_BELOW, 0x40 @;Appears underneath the sprite
.equ TEMPLATE_TARGET, 0x80

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
.equ BG_AURORA_BEAM, 0x14
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
.equ BG_AEROBLAST, 0x25
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
.equ BG_SNUGGLE_FOREVER, 0x46
.equ BG_MAX_LIGHTNING, 0x47
.equ BG_GARBAGE_FALLS, 0x48
.equ BG_HYPER_BEAM, 0x49
.equ BG_DYNAMAX_CANNON, 0x4A
.equ BG_AURA_SPHERE, 0x4B
.equ BG_CHLOROBLAST, 0x4C

@pals for pal fading tasks
.equ PAL_BG, 0x1
.equ PAL_ATK, 0x2
.equ PAL_DEF, 0x4
.equ PAL_ATK_PARTNER, 0x8
.equ PAL_DEF_PARTNER, 0x10
.equ PAL_ALL, 0x1f
.equ PAL_BG_4, 0x20
.equ PAL_BG_5, 0x40
.equ PAL_ALL_BANKS, 0x780
.equ PAL_PLAYER1, 0x80
.equ PAL_PLAYER2, 0x100
.equ PAL_OPPONENT1, 0x200
.equ PAL_OPPONENT2, 0x400

.equ RGB_BLACK, 0x0
.equ RGB_WHITE, 0x7FFF

@for sound commands
.equ SOUND_PAN_ABOVE, 0
.equ SOUND_PAN_BELOW, 127
.equ SOUND_PAN_ATTACKER, 0xC0
.equ SOUND_PAN_TARGET, 0x3F

@particle table
.macro animparticle offset size tag
.word \offset
.hword \size
.hword \tag
.endm

.macro animparticlepal offset tag hword
.word \offset
.hword \tag
.hword 0x0
.endm

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

.macro jumpifmoveturn jumpifmoveturnEQturn jumpifmoveturnEQPTR
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

.macro waitbgfadeout
.byte 0x16
.endm

.macro waitbgfadein
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

.macro soundcomplex songId panning lengthOfOne numTimes
.byte 0x1C
.hword \songId
.byte \panning
.byte \lengthOfOne
.byte \numTimes
.endm

.macro playsoundwait anim1Darg1 anim1Darg2 anim1Darg3
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

.macro pokespritetoBG2 animABank
.byte 0x22
.byte \animABank
.endm

.macro pokespritefromBG2 animBBank
.byte 0x23
.byte \animBBank
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
.equ AnimTask_ShakeMonInPlace, 0x8098CD1
.equ AnimTask_SwayMon, 0x8099705
.equ AnimTask_sprite_transform, 0x80DF9BD
.equ AnimTask_play_growling_cry, 0x80DD149
.equ AnimTask_BlendParticle, 0x80BAA21
.equ AnimTask_BlendPalInAndOutByTag, 0x8076289
.equ AnimTask_pal_fade, 0x80BA7F9
.equ AnimTask_pal_fade_complex, 0x80B9BDD
.equ AnimTask_BlendNonAttackerPalettes, 0x80bb7dd
.equ AnimTask_BlendBackground, 0x80adaa5
.equ AnimTask_HardwarePaletteFade, 0x80bab39
.equ AnimTask_screen_shake, 0x80B94B5
.equ AnimTask_prepare_moving_psychicBG, 0x80DE6F1
.equ AnimTask_IsTargetPlayerSide, 0x80DEAB5
.equ AnimTask_arg7_is_attacker_opponent, 0x80BB921
.equ AnimTask_arg7_is_in_contests, 0x80BC02D
.equ AnimTask_IsTargetSameSide, 0x80BC091
.equ AnimTask_scroll_background, 0x80BB82D
.equ AnimTask_steel_sprite, 0x80B86ED
.equ AnimTask_surf_wave, 0x80AB38D
.equ AnimTask_Splash, 0x80A8339
.equ AnimTask_create_haze, 0x80AFAE5
.equ AnimTask_SwaggerBulkUp, 0x80A84B5
.equ AnimTask_flash_anim, 0x80BB661
.equ AnimTask_RapidWhackSound, 0x80DD3DD
.equ AnimTask_SetGrayscaleOrOriginalPalette, 0x80B8A75
.equ AnimTask_ChargeBalls, 0x80AE541
.equ AnimTask_DestinyBondWhiteShadow, 0x80B6021
.equ AnimTask_SnatchOpposingMonMove, 0x80E392D
.equ AnimTask_SnatchPartnerMove, 0x80F1421
.equ AnimTask_ScaleMonAndRestore, 0x80998b1
.equ AnimTask_BlendExcept, 0x80ba83d
.equ AnimTask_LoadSandstormBackground, 0x80b4811
.equ AnimTask_RotateMonToSideAndRestore, 0x8099A79
.equ AnimTask_SetAnimAttackerAndTargetForEffectAtk, 0x80BC0FD
.equ AnimTask_TargetToEffectBattler, 0x80F1701
.equ AnimTask_Rollout, 0x80b4bd1
.equ AnimTask_ShakeTargetBasedOnMovePowerOrDmg, 0x8099bd5
.equ AnimTask_TranslateMonElliptical, 0x8098f85
.equ AnimTask_TranslateMonEllipticalRespectSide, 0x809907D
.equ AnimTask_AttackerFadeToInvisible, 0x80B78E1
.equ AnimTask_SetAttackerInvisibleWaitForSignal, 0x80bc12d
.equ AnimTask_InitAttackerFadeFromInvisible, 0x80b7a81
.equ AnimTask_AttackerFadeFromInvisible, 0x80b79dd
.equ AnimTask_StretchTargetUp, 0x80A9A21
.equ AnimTask_StretchAttackerUp, 0x80a9ab1
.equ AnimTask_PositionFissureBgOnBattler, 0x80b9801
.equ AnimTask_AnimateGustTornadoPalette, 0x80b194d
.equ AnimTask_DragonDanceWaver, 0x80b75e1
.equ AnimTask_BlendPalInAndOutByTag, 0x8076289
.equ AnimTask_BlendMonInAndOut, 0x807616D
.equ AnimTask_UproarDistortion, 0x80AA7C9
.equ AnimTask_FlailMovement, 0x80E0851
.equ AnimTask_TraceMonBlended, 0x80bab99
.equ AnimTask_SmokescreenImpact, 0x80de34d
.equ AnimTask_AttackerPunchWithTrace, 0x8077031
.equ AnimTask_ShakeAndSinkMon, 0x8098e91
.equ AnimTask_WindUpLunge, 0x809949D
.equ AnimTask_RotateMonSpriteToSide, 0x8099981
.equ AnimTask_RockMonBackAndForth, 0x80e0559
.equ AnimTask_FrozenIceCube, 0x80783FD
.equ AnimTask_GlareEyeDots, 0x80e21cd
.equ AnimTask_ScaryFace, 0x80a9f11
.equ AnimTask_ShakeTargetInPattern, 0x80adad9
.equ AnimTask_StatusClearedEffect, 0x80e2085
.equ AnimTask_InvertScreenColor, 0x80BA0E9
.equ AnimTask_DigDownMovement, 0x80b8e95
.equ AnimTask_SetAllNonAttackersInvisiblity, 0x80bb9b1
.equ AnimTask_FlashAnimTagWithColor, 0x80b9f6d
.equ AnimTask_VoltTackleBolt, 0x80aea11
.equ AnimTask_VoltTackleAttackerReappear, 0x80ae8a1
.equ AnimTask_StockpileDeformMon, 0x80df849
.equ AnimTask_CreateSpotlight, 0x80dedd9
.equ AnimTask_RemoveSpotlight, 0x80dee79
.equ AnimTask_CreateRaindrops, 0x80aabc1
.equ AnimTask_NightShadeClone, 0x80b54e9
.equ AnimTask_ElectricBolt, 0x80AE221
.equ AnimTask_GrudgeFlames, 0x80b68c9
.equ AnimTask_SpiteTargetShadow, 0x80b5aad
.equ AnimTask_DrillPeckHitSplats, 0x80b2869
.equ AnimTask_ScaryFace, 0x80a9f11
.equ AnimTask_TeeterDanceMovement, 0x80e4161
.equ AnimTask_DragonDanceWaver, 0x80b75e1
.equ AnimTask_AlphaFadeIn, 0x8076049
.equ AnimTask_FadeScreenBlue, 0x80A5FC1
.equ AnimTask_StartSinAnimTimer, 0x80ab101
.equ AnimTask_TransparentCloneGrowAndShrink, 0x80b3c79
.equ AnimTask_ExtrasensoryDistortion, 0x80b3a59
.equ AnimTask_SeismicTossBgAccelerateDownAtEnd, 0x80b51ed
.equ SoundTask_PlaySE2WithPanning, 0x80DD411
.equ SoundTask_LoopSEAdjustPanning, 0x80dcf39 @;Song Id, Source, Target Pan, Pan Increment, Num Times, Pan Increment Ofteness, Length of One
 
@launchtemplate

.equ Template_Hit, 0x83E7C08
.equ Template_FlashingHit, 0x83e7c98
.equ Template_WaterHit, 0x83E7C38
.equ Template_HornHit, 0x83e37bc
.equ Template_SlamHit, 0x83E3148
.equ Template_Fist, 0x83E6710
.equ Template_Claws, 0x83E79E8
.equ Template_Healing_Circle, 0x83E37A4
.equ Template_Ring, 0x83E4094
.equ Template_Explosion, 0x83E3F94
.equ Template_LusterPurgeHits, 0x83E7C50
.equ Template_Leer, 0x83FEF30
.equ Template_MiniFirePlume, 0x83E77D8
.equ Template_Pal_Fade, 0x83E7B24
.equ Template_SlideMonToOffset, 0x83d4e9c
.equ Template_SlideMonToOriginalPos, 0x83d4e84
.equ Template_HorizontalLunge, 0x83D4E54
.equ Template_VerticalDip, 0x83d4e6c
.equ Template_Teeth, 0x83E7930
.equ Template_FrenzyPlantRoot, 0x83e2ddc
.equ Template_BellyDrumNote, 0x83e3914
.equ Template_Electricity, 0x83E6088
.equ Template_ElectricSparkPlayer, 0x83e5fc4
.equ Template_SparkElectricityFlashing, 0x83e6070
.equ Template_ElectricPuff, 0x83e6204
.equ Template_ThunderboltOrb, 0x83e6058
.equ Template_DigMound, 0x83E7AC4
.equ Template_Lightning, 0x83E5F38
.equ Template_SmallBubblePair, 0x83e5ab0
.equ Template_HiddenPowerOrbScatter, 0x83e4294
.equ Template_RockFragment, 0x83e73cc
.equ Template_FireSpread, 0x83E5BF8
.equ Template_WishStar, 0x83ff168
.equ Template_FireSpiralOutward, 0x83e5dfc
.equ Template_EndureEnergy, 0x83E3604
.equ Template_LusterPurgeCircle, 0x83e7148
.equ Template_OutrageFlame, 0x83e772c
.equ Template_CuttingSlice, 0x83e3294
.equ Template_SuperpowerRock, 0x83E687C
.equ Template_FallingFeather, 0x83e6c00
.equ Template_BowMon, 0x83e3550
.equ Template_TrickBag, 0x83e2f60
.equ Template_ConstrictBinding, 0x83e2d0c
.equ Template_EllipticalGust, 0x83e6ae8
.equ Template_JaggedMusicNote, 0x83E4430
.equ Template_UproarRing, 0x83E4110
.equ Template_MovementWaves, 0x83E43F8
.equ Template_RockBlastRock, 0x83E7548
.equ Template_RockScatter, 0x83e7560
.equ Template_GrowingShockWaveOrb, 0x83E6290
.equ Tempate_RazorLeaf, 0x83e2c08
.equ Template_CurseGhost, 0x83E7698
.equ Template_KarateChop, 0x83e66e0
.equ Template_DragonBreathFire, 0x83e77a4
.equ Template_SweetScentPetal, 0x83FF324
.equ Template_CentredSparklingStars, 0x83E340C
.equ Template_SpiderWeb, 0x83e72dc
.equ Template_TearDrop, 0x83e7998
.equ Template_ShakeMonOrTerrain, 0x83e7b88
.equ Template_FallingRock, 0x83e73b4
.equ Template_BlackSmoke, 0x83fee18
.equ Template_GustToTarget, 0x83e6b1c
.equ Template_SmallDriftingBubbles, 0x83e5ac8
.equ Template_LargeFlameScatter,  0x83e5c88
.equ Template_IceCrystalHitLarge, 0x83E63E0
.equ Template_IceCrystalHitSmall, 0x83E63F8
.equ Template_IceCrystalSpiralInwardSmall, 0x83E6360
.equ Template_IceCrystalSpiralInwardLarge, 0x83E6348
.equ Template_PowerAbsorptionOrb, 0x83E2A58
.equ Template_GoldRing, 0x83E6EEC
.equ Template_ComplexPaletteBlend, 0x83e7b3c
.equ Template_KnockOffStrike, 0x83ff764
.equ Template_CrossImpact, 0x83e7c80
.equ Template_EyeSparkle, 0x83e42fc
.equ Template_MegaPunchKick, 0x83e67c0
.equ Template_DirtPlume, 0x83e7aac
.equ Template_FlyBallUp, 0x83e6bb8
.equ Template_WhirlwindLine, 0x83e6c84
.equ Template_DiveBall, 0x83e6d40
.equ Template_DiveWaterSplash, 0x83e6d7c
.equ Template_Whirlpool, 0x83e7420
.equ Template_AromatherapySmallFlower, 0x83e3024
.equ Template_AromatherapyBigFlower, 0x83e303c
.equ Template_BrickBreakWallShard, 0x83e6820
.equ Template_IceBeamOuterCrystal, 0x83e63a4
.equ Template_GrowingChargeOrb, 0x83e61d4
.equ Template_VoltTackleOrbSlide, 0x83e621c
.equ Template_BounceBallLand, 0x83e6cfc
.equ Template_GrantingStars, 0x83e33f4
.equ Template_FirePlume, 0x83e5ca0
.equ Template_FireSpin, 0x83e7438
.equ Template_FireBlastCross, 0x83e5de4
.equ Template_FireBlastRing, 0x83e5d94
.equ Template_WillOWispFire, 0x83e5ee4
.equ Template_FlatterSpotlight, 0x83ff3a0
.equ Template_SuperpowerOrb, 0x83e6864
.equ Template_ThinRingShrinking, 0x83E40C8
.equ Template_SwordsDanceBlade, 0x83e3bf8
.equ Template_IngrainRoot, 0x83e2dc4
.equ Template_IngrainOrb, 0x83e2e04
.equ Template_BlendThinRingExpanding, 0x83e40e0
.equ Template_ThunderWave, 0x83E60B8
.equ Template_LockOnTarget, 0x83e3518
.equ Template_LockOnMoveTarget, 0x83E3530
.equ Template_SwirlingDirt, 0x83E73E4
.equ Template_FlyingSand, 0x83E7450
.equ Template_OpeningEye, 0x83FEE5C
.equ Template_SlashSlice, 0x83e35a4
.equ Template_Moon, 0x83E3734
.equ Template_MoonlightSparkle, 0x83E3764
.equ Template_FastFlyingMusicNotes, 0x83E38C8
.equ Template_DragonDanceOrb, 0x83e7848
.equ Template_PsywaveRing, 0x83e59e8
.equ SpriteCB_AnimSpriteOnMonPos, 0x8075D9D
.equ Callback_TranslateAnimSpriteToTargetMonLocation, 0x8075DF5
.equ AnimTask_BendSprite, 0x8099981
.equ Template_FistAndFeet, 0x83E68AC

@OAM
.equ OAM_OFF_8x8, 0x83AC9C8
.equ OAM_OFF_16x16, 0x83AC9D0
.equ OAM_OFF_32x32, 0x83AC9D8
.equ OAM_OFF_64x64, 0x83AC9E0
.equ OAM_OFF_16x8, 0x83AC9E8
.equ OAM_OFF_32x8, 0x83AC9F0
.equ OAM_OFF_32x16, 0x83AC9F8
.equ OAM_OFF_64x32, 0x83ACA00
.equ OAM_OFF_8x16, 0x83ACA08
.equ OAM_OFF_8x32, 0x83ACA10
.equ OAM_OFF_16x32, 0x83ACA18
.equ OAM_OFF_32x64, 0x83ACA20
.equ OAM_NORMAL_8x8, 0x83ACA28
.equ OAM_NORMAL_16x16, 0x83ACA30
.equ OAM_NORMAL_32x32, 0x83ACA38
.equ OAM_NORMAL_64x64, 0x83ACA40
.equ OAM_NORMAL_16x8, 0x83ACA48
.equ OAM_NORMAL_32x8, 0x83ACA50
.equ OAM_NORMAL_32x16, 0x83ACA58
.equ OAM_NORMAL_64x32, 0x83ACA60
.equ OAM_NORMAL_8x16, 0x83ACA68
.equ OAM_NORMAL_8x32, 0x83ACA70
.equ OAM_NORMAL_16x32, 0x83ACA78
.equ OAM_NORMAL_32x64, 0x83ACA80
.equ OAM_DOUBLE_8x8, 0x83ACA88
.equ OAM_DOUBLE_16x16, 0x83ACA90
.equ OAM_DOUBLE_32x32, 0x83ACA98
.equ OAM_DOUBLE_64x64, 0x83ACAA0
.equ OAM_DOUBLE_16x8, 0x83ACAA8
.equ OAM_DOUBLE_32x8, 0x83ACAB0
.equ OAM_DOUBLE_32x16, 0x83ACAB8
.equ OAM_DOUBLE_64x32, 0x83ACAC0
.equ OAM_DOUBLE_8x16, 0x83ACAC8
.equ OAM_DOUBLE_8x32, 0x83ACAD0
.equ OAM_DOUBLE_16x32, 0x83ACAD8
.equ OAM_DOUBLE_32x64, 0x83ACAE0
.equ OAM_OFF_BLEND_8x8, 0x83ACAE8
.equ OAM_OFF_BLEND_16x16, 0x83ACAF0
.equ OAM_OFF_BLEND_32x32, 0x83ACAF8
.equ OAM_OFF_BLEND_64x64, 0x83ACB00
.equ OAM_OFF_BLEND_16x8, 0x83ACB08
.equ OAM_OFF_BLEND_32x8, 0x83ACB10
.equ OAM_OFF_BLEND_32x16, 0x83ACB18
.equ OAM_OFF_BLEND_64x32, 0x83ACB20
.equ OAM_OFF_BLEND_8x16, 0x83ACB28
.equ OAM_OFF_BLEND_8x32, 0x83ACB30
.equ OAM_OFF_BLEND_16x32, 0x83ACB38
.equ OAM_OFF_BLEND_32x64, 0x83ACB40
.equ OAM_NORMAL_BLEND_8x8, 0x83ACB48
.equ OAM_NORMAL_BLEND_16x16, 0x83ACB50
.equ OAM_NORMAL_BLEND_32x32, 0x83ACB58
.equ OAM_NORMAL_BLEND_64x64, 0x83ACB60
.equ OAM_NORMAL_BLEND_16x8, 0x83ACB68
.equ OAM_NORMAL_BLEND_32x8, 0x83ACB70
.equ OAM_NORMAL_BLEND_32x16, 0x83ACB78
.equ OAM_NORMAL_BLEND_64x32, 0x83ACB80
.equ OAM_NORMAL_BLEND_8x16, 0x83ACB88
.equ OAM_NORMAL_BLEND_8x32, 0x83ACB90
.equ OAM_NORMAL_BLEND_16x32, 0x83ACB98
.equ OAM_NORMAL_BLEND_32x64, 0x83ACBA0
.equ OAM_DOUBLE_BLEND_8x8, 0x83ACBA8
.equ OAM_DOUBLE_BLEND_16x16, 0x83ACBB0
.equ OAM_DOUBLE_BLEND_32x32, 0x83ACBB8
.equ OAM_DOUBLE_BLEND_64x64, 0x83ACBC0
.equ OAM_DOUBLE_BLEND_16x8, 0x83ACBC8
.equ OAM_DOUBLE_BLEND_32x8, 0x83ACBD0
.equ OAM_DOUBLE_BLEND_32x16, 0x83ACBD8
.equ OAM_DOUBLE_BLEND_64x32, 0x83ACBE0
.equ OAM_DOUBLE_BLEND_8x16, 0x83ACBE8
.equ OAM_DOUBLE_BLEND_8x32, 0x83ACBF0
.equ OAM_DOUBLE_BLEND_16x32, 0x83ACBF8
.equ OAM_DOUBLE_BLEND_32x64, 0x83ACC00

@calls

.equ SET_PSYCHIC_BG, 0x81D59BB
.equ UNSET_SCROLLING_BG, 0x81D59C7
.equ SET_SKY_BG, 0x81d59cf
.equ UNSET_SKY_BG, 0x81d59ff
.equ BURN_CHANCE_ANIM, 0x81D0950
.equ FREEZE_CHANCE_ANIM, 0x81D540A
.equ FREEZE_CHANCE_ANIM_DOUBLES, 0x81D548E
.equ PARALYZE_CHANCE_ANIM, 0x81D58D4
.equ POISON_BUBBLES, 0x81D575B
.equ HEALING_STARS, 0x81D569E
.equ HEALING_ANIM, 0x81D56C9
.equ SMALL_ABSORB_ANIM, 0x81CEC44
.equ RECOVER_LOAD_PARTICLES, 0x81D170D
.equ ABSORB_ANIM, 0x81CEEEC
.equ WATER_DIVE_ATTACK, 0x81C8CA2
.equ GROUND_DIGGING, 0x81CA90A
.equ BUFF_EFFECT, 0x81CB267
.equ CREATE_GROWLING_PARTICLES, 0x81CDB06
.equ SANDTOMB_TRAP_PARTICLES, 0x81D2DE3
.equ MIND_READER_EYE_SPIKE_EFFECT, 0x81CD1EF
.equ EXPLODING_ATTACKER2, 0x81C8EB2
.equ OUTRAGE_FLAMES, 0x81c95af
.equ STRING_SHOT_THREAD, 0x81d1504
.equ SPIDER_WEB_THREAD, 0x81d15a5
.equ FIRE_SPIN_TRAP_PARTICLES, 0x81C885F
.equ CREATE_COTTON_SPORES, 0x81D03B5
.equ HEALING_ANIM_TARGET, 0x81D5712
.equ SAND_ATTACK_DIRT, 0x81cdf72
.equ EMBER_FIRE_HIT, 0x81c7cd5
.equ PSYBEAM_RINGS, 0x81d0dd4
.equ DIVE_SET_UP_WATER_DROPLETS, 0x81d41b6
.equ ICE_BEAM_CRYSTALS, 0x81ce3fc
.equ DRAGON_CLAW_FIRE_SPIRAL, 0x81d31ad
.equ SONIC_BOOM_PROJECTILE, 0x81c7f12
.equ SEISMIC_TOSS_ROCK_SCATTER_1, 0x81d47c7
.equ SEISMIC_TOSS_ROCK_SCATTER_2, 0x81d4828

@supercommands

.macro unsetscrollingBG
loaddefaultBG
waitbgfadeout
setarg 0x7 0xffff
waitbgfadein
.endm

@particletags
.equ ANIM_TAG_BONE, 0x2710
.equ ANIM_TAG_SPARK, 0x2711
.equ ANIM_TAG_PENCIL, 0x2712
.equ ANIM_TAG_AIR_WAVE, 0x2713
.equ ANIM_TAG_UNUSED_ORB, 0x2714
.equ ANIM_TAG_SWORD, 0x2715
.equ ANIM_TAG_SEED, 0x2716
.equ ANIM_TAG_UNUSED_EXPLOSION, 0x2717
.equ ANIM_TAG_UNUSED_PINK_ORB, 0x2718
.equ ANIM_TAG_GUST, 0x2719
.equ ANIM_TAG_ICE_CUBE, 0x271a
.equ ANIM_TAG_SPARK_2, 0x271b
.equ ANIM_TAG_UNUSED_ORANGE, 0x271c
.equ ANIM_TAG_YELLOW_BALL, 0x271d
.equ ANIM_TAG_LOCK_ON, 0x271e
.equ ANIM_TAG_TIED_BAG, 0x271f
.equ ANIM_TAG_BLACK_SMOKE, 0x2720
.equ ANIM_TAG_BLACK_BALL, 0x2721
.equ ANIM_TAG_CONVERSION, 0x2722
.equ ANIM_TAG_UNUSED_GLASS, 0x2723
.equ ANIM_TAG_HORN_HIT, 0x2724
.equ ANIM_TAG_UNUSED_HIT, 0x2725	@ orange dot
.equ ANIM_TAG_UNUSED_HIT_2, 0x2726	@ blue dot
.equ ANIM_TAG_UNUSED_BLUE_SHARDS, 0x2727
.equ ANIM_TAG_UNUSED_CLOSING_EYE, 0x2728
.equ ANIM_TAG_UNUSED_WAVING_HAND, 0x2729
.equ ANIM_TAG_UNUSED_HIT_DUPLICATE, 0x272a
.equ ANIM_TAG_LEER, 0x272b
.equ ANIM_TAG_UNUSED_BLUE_BURST, 0x272c
.equ ANIM_TAG_SMALL_EMBER, 0x272d
.equ ANIM_TAG_GRAY_SMOKE, 0x272e
.equ ANIM_TAG_BLUE_STAR, 0x272f
.equ ANIM_TAG_UNUSED_BUBBLE_BURST, 0x2730
.equ ANIM_TAG_FIRE, 0x2731
.equ ANIM_TAG_UNUSED_SPINNING_FIRE, 0x2732
.equ ANIM_TAG_FIRE_PLUME, 0x2733
.equ ANIM_TAG_UNUSED_LIGHTNING, 0x2734
.equ ANIM_TAG_LIGHTNING, 0x2735
.equ ANIM_TAG_UNUSED_CLAW_SLASH, 0x2736	@left-parentheses trajectory
.equ ANIM_TAG_CLAW_SLASH, 0x2737
.equ ANIM_TAG_UNUSED_SCRATCH, 0x2738
.equ ANIM_TAG_UNUSED_SCRATCH_2, 0x2739
.equ ANIM_TAG_UNUSED_BUBBLE_BURST_2, 0x273a
.equ ANIM_TAG_ICE_CHUNK, 0x273b
.equ ANIM_TAG_UNUSED_GLASS_2, 0x273c
.equ ANIM_TAG_UNUSED_PINK_HEART, 0x273d
.equ ANIM_TAG_UNUSED_SAP_DRIP, 0x273e
.equ ANIM_TAG_UNUSED_SAP_DRIP_2, 0x273f
.equ ANIM_TAG_SPARKLE_1, 0x2740		@blue/green
.equ ANIM_TAG_SPARKLE_2, 0x2741
.equ ANIM_TAG_HUMANOID_FOOT, 0x2742
.equ ANIM_TAG_UNUSED_MONSTER_FOOT, 0x2743
.equ ANIM_TAG_UNUSED_HUMANOID_HAND, 0x2744
.equ ANIM_TAG_NOISE_LINE, 0x2745
.equ ANIM_TAG_UNUSED_YELLOW_UNK, 0x2746
.equ ANIM_TAG_UNUSED_RED_FIST, 0x2747
.equ ANIM_TAG_SLAM_HIT, 0x2748
.equ ANIM_TAG_UNUSED_RING, 0x2749
.equ ANIM_TAG_ROCKS, 0x274a
.equ ANIM_TAG_UNUSED_Z, 0x274b
.equ ANIM_TAG_UNUSED_YELLOW_UNK_2, 0x274c
.equ ANIM_TAG_UNUSED_AIR_SLASH, 0x274d
.equ ANIM_TAG_UNUSED_SPINNING_GREEN_ORBS, 0x274e
.equ ANIM_TAG_LEAF, 0x274f
.equ ANIM_TAG_FINGER, 0x2750
.equ ANIM_TAG_POISON_POWDER, 0x2751
.equ ANIM_TAG_UNUSED_BROWN_TRIANGLE, 0x2752
.equ ANIM_TAG_SLEEP_POWDER, 0x2753
.equ ANIM_TAG_STUN_SPORE, 0x2754
.equ ANIM_TAG_UNUSED_POWDER, 0x2755
.equ ANIM_TAG_SPARKLE_3, 0x2756
.equ ANIM_TAG_SPARKLE_4, 0x2757
.equ ANIM_TAG_MUSIC_NOTES, 0x2758
.equ ANIM_TAG_DUCK, 0x2759
.equ ANIM_TAG_MUD_SAND, 0x275a
.equ ANIM_TAG_ALERT, 0x275b
.equ ANIM_TAG_UNUSED_BLUE_FLAMES, 0x275c
.equ ANIM_TAG_UNUSED_BLUE_FLAMES_2, 0x275d
.equ ANIM_TAG_UNUSED_SHOCK, 0x275e
.equ ANIM_TAG_SHOCK, 0x275f
.equ ANIM_TAG_UNUSED_BELL, 0x2760
.equ ANIM_TAG_UNUSED_PINK_GLOVE, 0x2761
.equ ANIM_TAG_UNUSED_BLUE_LINES, 0x2762
.equ ANIM_TAG_UNUSED_IMPACT, 0x2763
.equ ANIM_TAG_UNUSED_IMPACT_2, 0x2764
.equ ANIM_TAG_UNUSED_RETICLE, 0x2765
.equ ANIM_TAG_BREATH, 0x2766
.equ ANIM_TAG_ANGER, 0x2767
.equ ANIM_TAG_UNUSED_SNOWBALL, 0x2768
.equ ANIM_TAG_UNUSED_VINE, 0x2769
.equ ANIM_TAG_UNUSED_SWORD, 0x276a
.equ ANIM_TAG_UNUSED_CLAPPING, 0x276b
.equ ANIM_TAG_UNUSED_RED_TUBE, 0x276c
.equ ANIM_TAG_AMNESIA, 0x276d
.equ ANIM_TAG_UNUSED_STRING, 0x276e
.equ ANIM_TAG_UNUSED_PENCIL, 0x276f
.equ ANIM_TAG_UNUSED_PETAL, 0x2770
.equ ANIM_TAG_BENT_SPOON, 0x2771
.equ ANIM_TAG_UNUSED_WEB, 0x2772
.equ ANIM_TAG_MILK_BOTTLE, 0x2773
.equ ANIM_TAG_COIN, 0x2774
.equ ANIM_TAG_UNUSED_CRACKED_EGG, 0x2775
.equ ANIM_TAG_UNUSED_HATCHED_EGG, 0x2776
.equ ANIM_TAG_UNUSED_FRESH_EGG, 0x2777
.equ ANIM_TAG_UNUSED_FANGS, 0x2778
.equ ANIM_TAG_UNUSED_EXPLOSION_2, 0x2779
.equ ANIM_TAG_UNUSED_EXPLOSION_3, 0x277a
.equ ANIM_TAG_UNUSED_WATER_DROPLET, 0x277b
.equ ANIM_TAG_UNUSED_WATER_DROPLET_2, 0x277c
.equ ANIM_TAG_UNUSED_SEED, 0x277d
.equ ANIM_TAG_UNUSED_SPROUT, 0x277e
.equ ANIM_TAG_UNUSED_RED_WAND, 0x277f
.equ ANIM_TAG_UNUSED_PURPLE_GREEN_UNK, 0x2780
.equ ANIM_TAG_UNUSED_WATER_COLUMN, 0x2781
.equ ANIM_TAG_UNUSED_MUD_UNK, 0x2782
.equ ANIM_TAG_RAIN_DROPS, 0x2783
.equ ANIM_TAG_UNUSED_FURY_SWIPES, 0x2784
.equ ANIM_TAG_UNUSED_VINE_2, 0x2785
.equ ANIM_TAG_UNUSED_TEETH, 0x2786
.equ ANIM_TAG_UNUSED_BONE, 0x2787
.equ ANIM_TAG_UNUSED_WHITE_BAG, 0x2788
.equ ANIM_TAG_UNUSED_UNKNOWN, 0x2789
.equ ANIM_TAG_UNUSED_PURPLE_CORAL, 0x278a
.equ ANIM_TAG_UNUSED_PURPLE_DROPLET, 0x278b
.equ ANIM_TAG_UNUSED_SHOCK_2, 0x278c
.equ ANIM_TAG_UNUSED_CLOSING_EYE_2, 0x278d
.equ ANIM_TAG_UNUSED_METAL_BALL, 0x278e
.equ ANIM_TAG_UNUSED_MONSTER_DOLL, 0x278f
.equ ANIM_TAG_UNUSED_WHIRLWIND, 0x2790
.equ ANIM_TAG_UNUSED_WHIRLWIND_2, 0x2791
.equ ANIM_TAG_UNUSED_EXPLOSION_4, 0x2792
.equ ANIM_TAG_UNUSED_EXPLOSION_5, 0x2793
.equ ANIM_TAG_UNUSED_TONGUE, 0x2794
.equ ANIM_TAG_UNUSED_SMOKE, 0x2795
.equ ANIM_TAG_UNUSED_SMOKE_2, 0x2796
.equ ANIM_TAG_IMPACT, 0x2797
.equ ANIM_TAG_CIRCLE_IMPACT, 0x2798
.equ ANIM_TAG_SCRATCH, 0x2799
.equ ANIM_TAG_CUT, 0x279a
.equ ANIM_TAG_SHARP_TEETH, 0x279b
.equ ANIM_TAG_RAINBOW_RINGS, 0x279c
.equ ANIM_TAG_ICE_CRYSTALS, 0x279d
.equ ANIM_TAG_ICE_SPIKES, 0x279e
.equ ANIM_TAG_HANDS_AND_FEET, 0x279f
.equ ANIM_TAG_MIST_CLOUD, 0x27a0
.equ ANIM_TAG_CLAMP, 0x27a1
.equ ANIM_TAG_BUBBLE, 0x27a2
.equ ANIM_TAG_ORBS, 0x27a3
.equ ANIM_TAG_WATER_IMPACT, 0x27a4
.equ ANIM_TAG_WATER_ORB, 0x27a5
.equ ANIM_TAG_POISON_BUBBLE, 0x27a6
.equ ANIM_TAG_TOXIC_BUBBLE, 0x27a7
.equ ANIM_TAG_SPIKES, 0x27a8
.equ ANIM_TAG_HORN_HIT_2, 0x27a9
.equ ANIM_TAG_AIR_WAVE_2, 0x27aa
.equ ANIM_TAG_SMALL_BUBBLES, 0x27ab
.equ ANIM_TAG_ROUND_SHADOW, 0x27ac
.equ ANIM_TAG_SUNLIGHT, 0x27ad
.equ ANIM_TAG_SPORE, 0x27ae
.equ ANIM_TAG_FLOWER, 0x27af
.equ ANIM_TAG_RAZOR_LEAF, 0x27b0
.equ ANIM_TAG_NEEDLE, 0x27b1
.equ ANIM_TAG_WHIRLWIND_LINES, 0x27b2
.equ ANIM_TAG_GOLD_RING, 0x27b3
.equ ANIM_TAG_PURPLE_RING, 0x27b4
.equ ANIM_TAG_BLUE_RING, 0x27b5
.equ ANIM_TAG_GREEN_LIGHT_WALL, 0x27b6
.equ ANIM_TAG_BLUE_LIGHT_WALL, 0x27b7
.equ ANIM_TAG_RED_LIGHT_WALL, 0x27b8
.equ ANIM_TAG_GRAY_LIGHT_WALL, 0x27b9
.equ ANIM_TAG_ORANGE_LIGHT_WALL, 0x27ba
.equ ANIM_TAG_BLACK_BALL_2, 0x27bb
.equ ANIM_TAG_PURPLE_GAS_CLOUD, 0x27bc
.equ ANIM_TAG_SPARK_H, 0x27bd
.equ ANIM_TAG_YELLOW_STAR, 0x27be
.equ ANIM_TAG_LARGE_FRESH_EGG, 0x27bf
.equ ANIM_TAG_SHADOW_BALL, 0x27c0
.equ ANIM_TAG_LICK, 0x27c1
.equ ANIM_TAG_UNUSED_VOID_LINES, 0x27c2
.equ ANIM_TAG_STRING, 0x27c3
.equ ANIM_TAG_WEB_THREAD, 0x27c4
.equ ANIM_TAG_SPIDER_WEB, 0x27c5
.equ ANIM_TAG_UNUSED_LIGHTBULB, 0x27c6
.equ ANIM_TAG_SLASH, 0x27c7
.equ ANIM_TAG_FOCUS_ENERGY, 0x27c8
.equ ANIM_TAG_SPHERE_TO_CUBE, 0x27c9
.equ ANIM_TAG_TENDRILS, 0x27ca
.equ ANIM_TAG_EYE, 0x27cb
.equ ANIM_TAG_WHITE_SHADOW, 0x27cc
.equ ANIM_TAG_TEAL_ALERT, 0x27cd
.equ ANIM_TAG_OPENING_EYE, 0x27ce
.equ ANIM_TAG_ROUND_WHITE_HALO, 0x27cf
.equ ANIM_TAG_FANG_ATTACK, 0x27d0
.equ ANIM_TAG_PURPLE_HAND_OUTLINE, 0x27d1
.equ ANIM_TAG_MOON, 0x27d2
.equ ANIM_TAG_GREEN_SPARKLE, 0x27d3
.equ ANIM_TAG_SPIRAL, 0x27d4
.equ ANIM_TAG_SNORE_Z, 0x27d5
.equ ANIM_TAG_EXPLOSION, 0x27d6
.equ ANIM_TAG_NAIL, 0x27d7
.equ ANIM_TAG_GHOSTLY_SPIRIT, 0x27d8
.equ ANIM_TAG_WARM_ROCK, 0x27d9
.equ ANIM_TAG_BREAKING_EGG, 0x27da
.equ ANIM_TAG_THIN_RING, 0x27db
.equ ANIM_TAG_PUNCH_IMPACT, 0x27dc
.equ ANIM_TAG_BELL, 0x27dd
.equ ANIM_TAG_MUSIC_NOTES_2, 0x27de
.equ ANIM_TAG_SPEED_DUST, 0x27df
.equ ANIM_TAG_TORN_METAL, 0x27e0
.equ ANIM_TAG_THOUGHT_BUBBLE, 0x27e1
.equ ANIM_TAG_MAGENTA_HEART, 0x27e2
.equ ANIM_TAG_ELECTRIC_ORBS, 0x27e3
.equ ANIM_TAG_CIRCLE_OF_LIGHT, 0x27e4
.equ ANIM_TAG_ELECTRICITY, 0x27e5
.equ ANIM_TAG_FINGER_2, 0x27e6
.equ ANIM_TAG_MOVEMENT_WAVES, 0x27e7
.equ ANIM_TAG_RED_HEART, 0x27e8
.equ ANIM_TAG_RED_ORB, 0x27e9
.equ ANIM_TAG_EYE_SPARKLE, 0x27ea
.equ ANIM_TAG_PINK_HEART, 0x27eb
.equ ANIM_TAG_ANGEL, 0x27ec
.equ ANIM_TAG_DEVIL, 0x27ed
.equ ANIM_TAG_SWIPE, 0x27ee
.equ ANIM_TAG_ROOTS, 0x27ef
.equ ANIM_TAG_ITEM_BAG, 0x27f0
.equ ANIM_TAG_JAGGED_MUSIC_NOTE, 0x27f1
.equ ANIM_TAG_POKEBALL, 0x27f2
.equ ANIM_TAG_SPOTLIGHT, 0x27f3
.equ ANIM_TAG_LETTER_Z, 0x27f4
.equ ANIM_TAG_RAPID_SPIN, 0x27f5
.equ ANIM_TAG_TRI_FORCE_TRIANGLE, 0x27f6
.equ ANIM_TAG_WISP_ORB, 0x27f7
.equ ANIM_TAG_WISP_FIRE, 0x27f8
.equ ANIM_TAG_GOLD_STARS, 0x27f9
.equ ANIM_TAG_ECLIPSING_ORB, 0x27fa
.equ ANIM_TAG_GRAY_ORB, 0x27fb
.equ ANIM_TAG_BLUE_ORB, 0x27fc
.equ ANIM_TAG_RED_ORB_2, 0x27fd
.equ ANIM_TAG_PINK_PETAL, 0x27fe
.equ ANIM_TAG_PAIN_SPLIT, 0x27ff
.equ ANIM_TAG_CONFETTI, 0x2800
.equ ANIM_TAG_GREEN_STAR, 0x2801
.equ ANIM_TAG_PINK_CLOUD, 0x2802
.equ ANIM_TAG_SWEAT_DROP, 0x2803
.equ ANIM_TAG_GUARD_RING, 0x2804
.equ ANIM_TAG_PURPLE_SCRATCH, 0x2805
.equ ANIM_TAG_PURPLE_SWIPE, 0x2806
.equ ANIM_TAG_TAG_HAND, 0x2807
.equ ANIM_TAG_SMALL_RED_EYE, 0x2808
.equ ANIM_TAG_HOLLOW_ORB, 0x2809
.equ ANIM_TAG_X_SIGN, 0x280a
.equ ANIM_TAG_BLUEGREEN_ORB, 0x280b
.equ ANIM_TAG_PAW_PRINT, 0x280c
.equ ANIM_TAG_PURPLE_FLAME, 0x280d
.equ ANIM_TAG_RED_BALL, 0x280e
.equ ANIM_TAG_SMELLINGSALT_EFFECT, 0x280f
.equ ANIM_TAG_METEOR, 0x2810
.equ ANIM_TAG_FLAT_ROCK, 0x2811
.equ ANIM_TAG_MAGNIFYING_GLASS, 0x2812
.equ ANIM_TAG_BROWN_ORB, 0x2813
.equ ANIM_TAG_METAL_SOUND_WAVES, 0x2814
.equ ANIM_TAG_FLYING_DIRT, 0x2815
.equ ANIM_TAG_ICICLE_SPEAR, 0x2816
.equ ANIM_TAG_HAIL, 0x2817
.equ ANIM_TAG_GLOWY_RED_ORB, 0x2818
.equ ANIM_TAG_GLOWY_GREEN_ORB, 0x2819
.equ ANIM_TAG_GREEN_SPIKE, 0x281a
.equ ANIM_TAG_WHITE_CIRCLE_OF_LIGHT, 0x281b
.equ ANIM_TAG_GLOWY_BLUE_ORB, 0x281c
.equ ANIM_TAG_POKEBLOCK, 0x281d
.equ ANIM_TAG_WHITE_FEATHER, 0x281e
.equ ANIM_TAG_SPARKLE_6, 0x281f
.equ ANIM_TAG_SPLASH, 0x2820
.equ ANIM_TAG_SWEAT_BEAD, 0x2821
.equ ANIM_TAG_UNUSED_GEM_1, 0x2822
.equ ANIM_TAG_UNUSED_GEM_2, 0x2823
.equ ANIM_TAG_UNUSED_GEM_3, 0x2824
.equ ANIM_TAG_SLAM_HIT_2, 0x2825
.equ ANIM_TAG_RECYCLE, 0x2826
.equ ANIM_TAG_UNUSED_RED_PARTICLES, 0x2827
.equ ANIM_TAG_PROTECT, 0x2828
.equ ANIM_TAG_DIRT_MOUND, 0x2829
.equ ANIM_TAG_SHOCK_3, 0x282a
.equ ANIM_TAG_WEATHER_BALL, 0x282b
.equ ANIM_TAG_BIRD, 0x282c
.equ ANIM_TAG_CROSS_IMPACT, 0x282d
.equ ANIM_TAG_SLASH_2, 0x282e
.equ ANIM_TAG_WHIP_HIT, 0x282f
.equ ANIM_TAG_BLUE_RING_2, 0x2830
.equ ANIM_TAG_WOOD, 0x2831
.equ ANIM_TAG_FLASH_CANNON_BALL, 0x2832
.equ ANIM_TAG_POISON_JAB, 0x2833
.equ ANIM_TAG_POWER_GEM, 0x2834
.equ ANIM_TAG_SHELL_RIGHT, 0x2835
.equ ANIM_TAG_SHELL_LEFT, 0x2836
.equ ANIM_TAG_RAZOR_SHELL, 0x2837
.equ ANIM_TAG_HYDRO_PUMP, 0x2838
.equ ANIM_TAG_WATER_GUN, 0x2839
.equ ANIM_TAG_BRINE, 0x283A
.equ ANIM_TAG_STEALTH_ROCK, 0x283B
.equ ANIM_TAG_STONE_EDGE, 0x283C
.equ ANIM_TAG_GEAR, 0x283D
.equ ANIM_TAG_ASSURANCE_HAND, 0x283E
.equ ANIM_TAG_ACUPRESSURE_FINGER, 0x283F
.equ ANIM_TAG_PUNISHMENT_BLADES, 0x2840
.equ ANIM_TAG_BEES, 0x2841
.equ ANIM_TAG_WISHIWASHI_FISH, 0x2842
.equ ANIM_TAG_ZYGARDE_HEXES, 0x2843
.equ ANIM_TAG_AURA_SPHERE, 0x2844
.equ ANIM_TAG_ENERGY_BALL, 0x2845
.equ ANIM_TAG_MEGA_RAINBOW, 0x2846
.equ ANIM_TAG_MEGA_STONE, 0x2847
.equ ANIM_TAG_MEGA_SYMBOL, 0x2848
.equ ANIM_TAG_OMEGA_STONE, 0x2849
.equ ANIM_TAG_ALPHA_STONE, 0x284A
.equ ANIM_TAG_BERRY_NORMAL, 0x284B
.equ ANIM_TAG_BERRY_EATEN, 0x284C
.equ ANIM_TAG_DRAGON_ASCENT, 0x284D
.equ ANIM_TAG_PINK_DIAMOND, 0x284E
.equ ANIM_TAG_STEAM_ERUPTION, 0x284F
.equ ANIM_TAG_CONFIDE, 0x2850
.equ ANIM_TAG_VERTICAL_HEX, 0x2851
.equ ANIM_TAG_POWER_TRICK, 0x2852
.equ ANIM_TAG_CHAIN_LINK, 0x2853
.equ ANIM_TAG_ANCHOR, 0x2854
.equ ANIM_TAG_HORSESHOE_SIDE_FIST, 0x2855
.equ ANIM_TAG_DRAGON_ASCENT_FOE, 0x2856
.equ ANIM_TAG_CRAFTY_SHIELD, 0x2857
.equ ANIM_TAG_QUICK_GUARD_HAND, 0x2858
.equ ANIM_TAG_BLACEPHALON_HEAD, 0x2859
.equ ANIM_TAG_FAIRY_LOCK_CHAINS, 0x285A
.equ ANIM_TAG_IONS, 0x285B
.equ ANIM_TAG_CHOP, 0x285C
.equ ANIM_TAG_HEART_STAMP, 0x285D
.equ ANIM_TAG_HORN_LEECH, 0x285E
.equ ANIM_TAG_STEAMROLLER, 0x285F
.equ ANIM_TAG_HOOPA_HAND, 0x2860
.equ ANIM_TAG_HOOPA_RING, 0x2861
.equ ANIM_TAG_UNAVAILABLE_1, 0x2862
.equ ANIM_TAG_UNAVAILABLE_2, 0x2863
.equ ANIM_TAG_METAL_BITS, 0x2864
.equ ANIM_TAG_SMALL_ROCK, 0x2865
.equ ANIM_TAG_SPIRIT_ARROW, 0x2866
.equ ANIM_TAG_ULTRA_BURST_SYMBOL, 0x2867
.equ ANIM_TAG_Z_MOVE_SYMBOL, 0x2868
.equ ANIM_TAG_REALLY_BIG_ROCK, 0x2869
.equ ANIM_TAG_COCOON, 0x286A
.equ ANIM_TAG_CORKSCREW, 0x286B
.equ ANIM_TAG_HAVOC_SPEAR, 0x286C
.equ ANIM_TAG_PURPLE_DRAKE, 0x286D
.equ ANIM_TAG_ABILITY_POP_UP, 0x286E
.equ ANIM_TAG_MUD_BOMB, 0x286F
.equ ANIM_TAG_BRANCH, 0x2870
.equ ANIM_TAG_APPLE, 0x2871
.equ ANIM_TAG_OBSTRUCT_CROSS, 0x2872
.equ ANIM_TAG_POISON_COLUMN, 0x2873
.equ ANIM_TAG_GARBAGE_COLUMN, 0x2874
.equ ANIM_TAG_LARGE_SPIKE, 0x2875
.equ ANIM_TAG_DRAGON_PULSE_RING, 0x2876
.equ ANIM_TAG_STONE_PILLAR, 0x2877
.equ ANIM_TAG_MUSHROOM, 0x2878
.equ ANIM_TAG_GOLDEN_APPLE, 0x2879
.equ ANIM_TAG_ICE_ROCK, 0x287A
.equ ANIM_TAG_TORNADO, 0x287B
.equ ANIM_TAG_STRAIGHT_BEAM, 0x287C
.equ ANIM_TAG_DREEPY, 0x287D
.equ ANIM_TAG_ICE_ROCK_SINGLE, 0x287E
.equ ANIM_TAG_STONE_PILLAR_MULTI, 0x287F
.equ ANIM_TAG_HOOPA_RING_LARGE, 0x2880
.equ ANIM_TAG_OMEGA_SYMBOL, 0x2881
.equ ANIM_TAG_ALPHA_SYMBOL, 0x2882

//New Anim
.equ Template_ElectricStatic, 0x83E6204
.equ SparksSound, 0x70
.equ ThunderSound, 0xD6
.equ Template_ElectricSparkPlayer2, 0x83E6070   @Temp271B_A
.equ Template_SpotlightTarget, 0x83FF3A0
.equ AnimTask_FadeWhiteOrBlack, 0x80BAB39
.equ AnimTask_PlayUserCry, 0x80DD333    @PlayUserCry
.equ Template_Sparkle, 0x83E6EA4 
.equ Template_Temp27AB_G, 0x83E5B70
.equ AnimTask_GetWeather, 0x80E44EB
.equ Template_GraySmoke, 0x83E36A8
.equ CoinSound, 0xA7
