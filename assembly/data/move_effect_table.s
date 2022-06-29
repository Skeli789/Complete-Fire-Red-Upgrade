.thumb
.text
.align 2

.include "../asm_defines.s"
/*
move_effect_tables.s
	table of move effect IDs
*/

.global gBattleScriptsForMoveEffects
.global gSetStatusMoveEffects
.global gStatLoweringMoveEffects
.global gConfusionMoveEffects
.global gMoveEffectsThatIgnoreWeaknessResistance

gBattleScriptsForMoveEffects:
.word 0x81D6926		@EFFECT_HIT
.word BS_001_SetSleep
.word BS_002_SetPoisonChance
.word BS_003_DrainHP
.word BS_004_SetBurnChance
.word BS_005_SetFreezeChance
.word BS_006_SetParalyzeChance
.word BS_007_Explode
.word BS_008_DreamEater
.word BS_009_MirrorMove
.word BS_010_RaiseUserAtk1
.word BS_011_RaiseUserDef1
.word BS_012_RaiseUserSpd1
.word BS_013_RaiseUserSpAtk1
.word BS_014_RaiseUserSpDef1
.word BS_015_RaiseUserAcc1
.word BS_016_RaiseUserEvsn1
.word BS_017_NeverMiss
.word BS_018_LowerTargetAtk1
.word BS_019_LowerTargetDef1
.word BS_020_LowerTargetSpd1
.word BS_021_LowerTargetSpAtk1
.word BS_022_LowerTargetSpDef1
.word BS_023_LowerTargetAcc1
.word BS_024_LowerTargetEvsn1
.word BS_025_Haze
.word BS_026_Bide
.word BS_027_Thrash
.word BS_028_Roar
.word BS_029_HitTwoToFiveTimes
.word BS_030_Conversion
.word BS_031_Flinch
.word BS_032_Recover
.word BS_033_SetBadPoison
.word BS_034_PayDay
.word BS_035_LightScreen
.word BS_036_TriAttack
.word BS_037_SetRest
.word BS_038_OHK0
.word BS_039_RazorWind
.word BS_040_SuperFang
.word BS_041_DragonRage
.word BS_042_TrapAndDamage
.word BS_043_HighCritRatio
.word BS_044_HitTwice
.word BS_045_HighJumpKick
.word BS_046_Mist
.word BS_047_FocusEnergy
.word BS_048_25PercentRecoil
.word BS_049_SetConfusion
.word BS_050_RaiseUserAtk2
.word BS_051_RaiseUserDef2
.word BS_052_RaiseUserSpd2
.word BS_053_RaiseUserSpAtk2
.word BS_054_RaiseUserSpDef2
.word BS_055_RaiseUserAcc2
.word BS_056_RaiseUserEvsn2
.word BS_057_Transform
.word BS_058_LowerTargetAtk2
.word BS_059_LowerTargetDef2
.word BS_060_LowerTargetSpd2
.word BS_061_LowerTargetSpAtk2
.word BS_062_LowerTargetSpDef2
.word BS_063_LowerTargetAcc2
.word BS_064_LowerTargetEvsn2
.word BS_065_Reflect
.word BS_066_SetPoison
.word BS_067_SetParalyze
.word BS_068_LowerTargetAtk1Chance
.word BS_069_LowerTargetDef1Chance
.word BS_070_LowerTargetSpd1Chance
.word BS_071_LowerTargetSpAtk1Chance
.word BS_072_LowerTargetSpDef1Chance
.word BS_073_LowerTargetAcc1Chance
.word BS_074_LowerTargetEvsn1Chance
.word BS_075_SkyAttack
.word BS_076_SetConfusionChance
.word BS_077_LowerTargetSpDef2Chance
.word BS_078_Blank
.word BS_079_Substitute
.word BS_080_Recharge
.word BS_081_Rage
.word BS_082_Mimic
.word BS_083_Metronome
.word BS_084_LeechSeed
.word BS_085_Splash
.word BS_086_Disable
.word BS_087_SeismicToss
.word BS_088_Psywave
.word BS_089_Counter
.word BS_090_Encore
.word BS_091_PainSplit
.word BS_092_Snore
.word 0x81d74d6		@EFFECT_CONVERSION_2
.word BS_094_LockOn
.word BS_095_Sketch
.word BS_096_RaiseAttackerSpd1Chance
.word BS_097_SleepTalk
.word BS_098_DestinyBond
.word BS_099_Flail
.word BS_100_Spite
.word BS_101_FalseSwipe
.word BS_102_HealBell
.word BS_103_PriorityMove
.word BS_104_TripleKick
.word BS_105_StealItem
.word BS_106_MeanLook
.word BS_107_Nightmare
.word BS_108_Minimize
.word BS_109_Curse
.word BS_110_RaiseAttackerSpAtk1Chance
.word BS_111_Protect
.word BS_112_Spikes
.word BS_113_Foresight
.word BS_114_PerishSong
.word BS_115_Sandstorm
.word BS_116_Blank
.word BS_117_Rollout
.word BS_118_Swagger
.word BS_119_FuryCutter
.word BS_120_Attract
.word BS_121_Blank
.word BS_122_Present
.word BS_123_Blank
.word BS_124_Safeguard
.word BS_125_BurnUp
.word BS_126_Magnitude
.word BS_127_BatonPass
.word BS_128_Pursuit
.word BS_129_RapidSpin
.word BS_130_Sonicboom
.word BS_131_Blank
.word BS_132_MorningSun
.word BS_133_Blank	@was synthesis
.word BS_134_Blank	@was moonlight
.word BS_135_AttackerRaiseDef2Chance
.word BS_136_RainDance
.word BS_137_SunnyDay
.word BS_138_AttackerRaiseDef1Chance
.word BS_139_AttackerRaiseAtk1Chance
.word BS_140_AttackerRaiseAllStatsChance
.word BS_141_AttackerHitRaiseHigherOffensesDefenses
.word BS_142_BellyDrum
.word BS_143_PsychUp
.word BS_144_MirrorCoat
.word BS_145_SkullBash
.word BS_146_Twister
.word BS_147_Earthquake
.word BS_148_FutureSight
.word BS_149_Gust
.word BS_150_Splinters
.word BS_151_Solarbeam
.word BS_152_Thunder
.word BS_153_Teleport
.word BS_154_BeatUp
.word BS_155_Fly
.word BS_156_DefenseCurl
.word BS_157_SpringtideStorm	@was softboiled
.word BS_158_FakeOut
.word BS_159_Uproar
.word BS_160_Stockpile
.word BS_161_SpitUp
.word BS_162_Swallow
.word BS_163_Blank
.word BS_164_SetHail
.word BS_165_Torment
.word BS_166_Flatter
.word BS_167_SetBurn
.word BS_168_Memento
.word BS_169_Blank
.word BS_170_FocusPunch
.word BS_171_SmellingSalt
.word BS_172_FollowMe
.word BS_173_NaturePower
.word BS_174_Charge
.word BS_175_Taunt
.word BS_176_HelpingHand
.word BS_177_Trick
.word BS_178_RolePlay
.word BS_179_Wish
.word BS_180_Assist
.word BS_181_Ingrain
.word BS_182_Superpower
.word BS_183_MagicCoat
.word BS_184_Recycle
.word BS_185_Blank	@was revenge
.word BS_186_BrickBreak
.word BS_187_Yawn
.word BS_188_KnockOff
.word BS_189_Endeavor
.word BS_190_Blank	@was eruption
.word BS_191_SkillSwap
.word BS_192_Imprison
.word BS_193_Refresh
.word BS_194_Grudge
.word BS_195_Snatch
.word BS_196_Blank	@was low kick
.word BS_197_SecretPower
.word BS_198_RaiseUserAtkSpAtk
.word BS_199_RaiseUserAtkAcc
.word BS_200_RaiseUserDefSpeed	@was blaze kick
.word BS_201_MudSport
.word BS_202_VenomDrench	@was poison fang
.word BS_203_PlayNice	@was weather ball
.word BS_204_Overheat
.word BS_205_Tickle
.word BS_206_CosmicPower
.word BS_207_ExtremeEvoBoost	@was sky uppercut
.word BS_208_BulkUp
.word BS_209_BadPoisonChance
.word BS_210_WaterSport
.word BS_211_CalmMind
.word BS_212_DragonDance
.word BS_213_StatSwapSplitters
.word BS_214_Blank
.word BS_215_Blank
.word BS_216_Blank
.word BS_217_Blank
.word BS_218_MeFirst
.word BS_219_DestroyBerry
.word BS_220_NaturalGift
.word BS_221_SmackDown
.word BS_222_DamageResetTargetStatChanges
.word BS_223_RelicSong
.word BS_224_Blank
.word BS_225_Blank
.word BS_226_Terrain
.word BS_227_Pledges
.word BS_228_FieldEffects
.word BS_229_Fling
.word BS_230_Feint
.word BS_231_AttackBlockers
.word BS_232_TypeChangers
.word BS_233_HealTarget
.word BS_234_TopsyTurvyElectrify
.word BS_235_FairyLockHappyHourCelebrateHoldHands
.word BS_236_InstructAfterYouQuash
.word BS_237_SuckerPunch
.word BS_238_Blank
.word BS_239_TeamEffectsAndMagnetRise
.word BS_240_Camouflage
.word BS_241_FlameBurst
.word BS_242_LastResort
.word BS_243_DamageSetTerrain
.word BS_244_Teatime
.word BS_245_Poltergeist
.word BS_246_SkyDrop
.word BS_247_Blank
.word BS_248_Blank
.word BS_249_Blank
.word BS_250_Blank
.word BS_251_Blank
.word BS_252_Blank
.word BS_253_MaxMove
.word BS_254_Blank
.word BS_255_Blank

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

gSetStatusMoveEffects:
.byte EFFECT_SLEEP
.byte EFFECT_TOXIC
.byte EFFECT_POISON
.byte EFFECT_PARALYZE
.byte EFFECT_WILL_O_WISP
.byte EFFECT_YAWN
.byte MOVE_EFFECT_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

gStatLoweringMoveEffects:
.byte EFFECT_ATTACK_DOWN
.byte EFFECT_DEFENSE_DOWN
.byte EFFECT_SPEED_DOWN
.byte EFFECT_SPECIAL_ATTACK_DOWN
.byte EFFECT_SPECIAL_DEFENSE_DOWN
.byte EFFECT_ACCURACY_DOWN
.byte EFFECT_EVASION_DOWN
.byte EFFECT_ATTACK_DOWN_2
.byte EFFECT_DEFENSE_DOWN_2
.byte EFFECT_SPEED_DOWN_2
.byte EFFECT_SPECIAL_ATTACK_DOWN_2
.byte EFFECT_SPECIAL_DEFENSE_DOWN_2
.byte EFFECT_ACCURACY_DOWN_2
.byte EFFECT_EVASION_DOWN_2
.byte EFFECT_TICKLE
.byte EFFECT_PLAY_NICE
.byte EFFECT_VENOM_DRENCH
.byte MOVE_EFFECT_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

gConfusionMoveEffects:
.byte EFFECT_CONFUSE
.byte EFFECT_SWAGGER
.byte EFFECT_FLATTER
.byte MOVE_EFFECT_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

gMoveEffectsThatIgnoreWeaknessResistance:
.byte EFFECT_BIDE
.byte EFFECT_SUPER_FANG
.byte EFFECT_DRAGON_RAGE
.byte EFFECT_SONICBOOM
.byte EFFECT_LEVEL_DAMAGE
.byte EFFECT_PSYWAVE
.byte EFFECT_MEMENTO @Final Gambit
.byte EFFECT_ENDEAVOR
.byte EFFECT_PAIN_SPLIT
.byte EFFECT_COUNTER
.byte EFFECT_MIRROR_COAT
.byte MOVE_EFFECT_TABLES_TERMIN
