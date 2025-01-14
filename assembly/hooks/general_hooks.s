.text
.thumb
.align 2

.include "../asm_defines.s"

.global BuildTrainerPartyHook

.pool
BuildTrainerPartyHook:
	bl BuildTrainerPartySetup
	ldr r0, =0x800FF98 | 1
	bx r0

.pool
@0x800D2CC with r0
AIHandleItemUseHook:
	bl HandleNewBattleRamClearBeforeBattle
	mov r0, #0xF
	bl BattleAI_HandleItemUseBeforeAISetup
	ldr r0, =0x2022B54
	str r4, [r0]
	ldr r0, =0x800D2D4 | 1
	bx r0

EmitChooseMoveHook:
	mov r3, r5
	bl EmitChooseMove
	pop {r4-r6,pc}

.pool
@0x80140C8 with r0
AIEmitChooseActionHook:
	mov r0, r5
	bl ShouldAIChooseAction
	cmp r0, #0x0
	beq SkipChooseAction
	ldr r0, =0x8014114 | 1
	bx r0

SkipChooseAction:
	ldr r0, =0x8014C4E | 1
	bx r0

.pool
@0x802EF90 with r0
SwitchMoveDataHook:
	bl HandleMoveSwitchingUpdate
	ldr r0, =0x802F2C8 | 1
	bx r0

.pool
@0x8015438 with r0
TurnValuesCleanUpAdditionHook:
	bl CleanUpExtraTurnValues
	pop {r4-r7, pc}

.pool
@0x0804AA1E with r1
GetProperBallIdHook1:
	lsl r0, #0x18
	lsr r0, #0x18
	mov r5, r0
	ldr r1, =0x804AA28 | 1
	bx r1

.pool
@0x080EF3A8 with r1
GetProperBallIdHook2:
	lsl r0, #0x18
	lsr r0, #0x18
	ldr r1, =0x80EF3B0 | 1
	bx r1

.pool
@0x801645E with r0
GetPokeBallBattleScriptHook:
	push {r3}
	bl TryGetPokeBallBattleScript
	cmp r0, #0x0
	pop {r3}
	bne SuccessfullBallBattleScript
	mov r1, r3
	ldr r0, =0x801649C | 1 @Check Poke Doll
	bx r0

SuccessfullBallBattleScript:
	ldr r0, =0x80164FC | 1
	bx r0

.pool
@0x804B946 with r0
OakIntroPokeballFixHook:
	bl OakIntroPokeballFix
	ldr r1, =0x804B954 | 1
	bx r1

.pool
@0x804BBD8 with r0
TradePokeballFixHook:
	bl OakIntroPokeballFix
	ldr r1, =0x804BBE6 | 1
	bx r1

.pool
@0x80F187C with r2
ShinyAnimFixHook:
	ldr r2, =GetBattlerSpriteCoord
	bl bxr2
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	push {r2}
	bl LoadShinyStarsSpriteTiles
	pop {r2}
	ldr r0, =0x80F1884 | 1
	bx r0

.pool
@0x8014450 with r0
PartyMenuSwitchingHook:
	bl PartyMenuSwitchingUpdate
	ldr r0, =0x80146AC | 1
	bx r0

.pool
@0x8125EEE with r1
PartyMenuAfterCancelLearnMoveHook:
	bl InitPartyMenuAfterCancellingLearnMove
	ldr r0, =0x8125F02 | 1
	bx r0

.pool
@0x80EFEE4 with r0
CriticalCaptureAnimHook:
	push {r1}
	bl IsCriticalCapture
	pop {r1}
	cmp r0, #0x0
	beq NormalBallThrowReturn
	bl IsCriticalCaptureSuccess
	cmp r0, #0x0
	beq BallThrowFailureReturn
	ldr r0, =0x80EFF14 | 1 @Success
	bx r0

BallThrowFailureReturn:
	ldr r0, =0x80EFEF0 | 1 @Break out
	bx r0

NormalBallThrowReturn:
	ldr r0, =0x2024018 @gBattleSpritesDataPtr
	ldr r0, [r0]
	ldr r0, [r0, #0x8]
	ldrb r0, [r0, #0x8]
	ldr r2, =0x80EFEEC | 1
	bx r2

.pool
@0x804AA2C with r0
PokeBallThrowSoundHook:
	mov r0, #0x36 @Throw Poke Ball
	ldr r1, =PlaySE
	bl bxr1
	ldr r0, =0x804AA68 | 1
	bx r0

.pool
@0x804AB04 with r1
TrainersThrowPokeBallsHook1:
	add r0, #24 @X-Offset to the right thrown from
	strh r0, [r4, #0x20]
	mov r0, r6
	mov r1, #1
	ldr r2, =GetBattlerSpriteCoord
	bl bxr2
	lsl r0, #0x18
	lsr r0, #0x18
	sub r0, #12 @Y-Offset upwards thrown from
	ldr r1, =0x0804AB14 | 1
	bx r1

.pool
@0x804B80E with r1
TrainersThrowPokeBallsHook2:
	mov r0, r4
	ldr r1, =GetBattlerPosition
	bl bxr1
	mov r1, #2 @BIT_FLANK
	and r0, r1
	cmp r0, #0
	beq ReleaseMon1FromBall @Left mon
	ldr r0, =0x0804B81C | 1 @Release mon 2
	bx r0

ReleaseMon1FromBall:
	ldr r0, =0x0804B828 | 1
	bx r0

.pool
@0x80A1E2C with r0
DoubleWildPokeBallItemUseFixHook:
	mov r0, r4
	bl DoubleWildPokeBallItemUseFix
	cmp r0, #0x0
	bne DoubleWildPokeBallItemUseFixEnd

	ldr r0, =0x203AD30 @Var 0x800E
	ldrh r0, [r0]
	mov r1, #0x1
	bl RemoveBagItem
	ldr r0, =0x80A1E36 | 1
	bx r0

DoubleWildPokeBallItemUseFixEnd:
	pop {r4, pc}

RemoveBagItem:
	ldr r3, =0x809A1D8 | 1
bxr3:
	bx r3

.pool
@0x802D9D8 with r0
DoubleWildDexHook1:
	push {r4-r5,lr}
	sub sp, #0x18
	bl LoadTargetPokedexSpecies
	ldr r1, =0x0802D9E6 | 1
	bx r1

.pool
@0x802DAEC with r0
DoubleWildDexHook2:
	mov r0, r5
	bl CreateCapturedMonDexPic
	ldr r0, =0x802DB0E | 1
	bx r0

.pool
@0x0802DE4E with r0
CaptureExperienceEvolutionHook:
	ldrb r0, [r0]
	cmp r0, #0x0
	beq NoEvolutionPostBattle
	ldr r0, =BATTLE_OUTCOME
	ldrb r0, [r0]
	cmp r0, #0x1 @Won battle
	beq EvolutionPostBattle
	cmp r0, #0x7 @Caught
	beq EvolutionPostBattle

NoEvolutionPostBattle:
	ldr r0, =0x8015A52 | 1
	bx r0

EvolutionPostBattle:
	ldr r0, =0x8015A6C | 1
	bx r0	

.pool
@0x8013D14 with r1
TrainerSlidingEndTurnHook:
	mov r1, r10
	str r0, [r1]
	bl CheckLastMonLowHPSlide
	bl HandleDeadRaidMonAndDeadPlayer
	ldr r2, =Random
	bl bxr2
	ldr r1, =0x8013D1C | 1
	bx r1

.pool
@0x80813B8 with r0
LoadProperMusicForLinkBattlesHook:
	bl LoadProperMusicForLinkBattles
	ldr r1, =0x80813C6 | 1
	bx r1

.pool
@0x806E454 with r1
ExpandedVarsHook:
	push {r4-r6, lr}
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r6, r4
	mov r0, r4
	bl GetExpandedVarPointer
	cmp r0, #0x1
	beq VarReturn0
	cmp r0, #0x0
	bne ExpandedVarsPop
	ldr r0, =0x806E45C | 1
	bx r0

ExpandedVarsPop:
	pop {r4-r6,pc}

VarReturn0:
	ldr r0, =0x806E462 | 1
	bx r0

.pool
@0x806E5C0 with r1
ExpandedFlagsHook:
	push {r4-r6, lr}
	lsl r5, r0, #0x10
	lsr r4, r5, #0x10
	mov r6, r4
	mov r0, r4
	bl GetExpandedFlagPointer
	cmp r0, #0x0
	bne ExpandedVarsPop
	ldr r0, =0x806E5C8 | 1
	bx r0

.pool
@0x80142D8 with r0
DoubleWildUseItemRunFixHook:
	ldr r5, =ACTIVE_BATTLER
	ldrb r4, [r5]
	bl CheckCantMoveThisTurn
	cmp r0, #0x0
	beq SkipEncoreReturn
	ldr r0, =0x801460E | 1
	bx r0

SkipEncoreReturn:
	ldr r0, =0x801432C | 1
	bx r0

.pool
@0x806FC84 with r0
LinkBattleSaveHook:
	bl SaveDataAfterLinkBattle
	ldr r1, =0x806FCE6 | 1
	bx r1

.pool
@0x805415E with r0
LinkTradeSaveHook:
	bl SaveDataAfterLinkTrade
	ldr r1, =gMain
	mov r2, #0x87
	lsl r2, #0x3
	add r1, r2
	mov r2, #0x0
	ldr r3, =0x8054170 | 1
	bx r3

.pool
@0x8054A60 with r0
NewGameSaveClearHook:
	push {r4-r6, lr}
	mov r6, r8
	push {r6}
	sub sp, sp, #0x8
	bl NewGameWipeNewSaveData
	ldr r0, =0x8054A68 | 1
	bx r0

.pool
@0x80CEDD4 with r1
EvolutionMovesHook:
	ldrb r1, [r5, #0x10]
	mov r0, r9
	bl MonTryLearningNewMoveAfterEvolution
	ldr r1, =0x80CEDDC | 1
	bx r1

.pool
@0x81058DC with r0
PokedexLoadAlternateHeightHook:
	lsr r7, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r8, r3
	mov r5, r1 @Save species
	mov r0, r1
	mov r1, #0x0 @PKDX_GET_HEIGHT
	bl TryGetAlternateSpeciesSize
	cmp r0, #0x0
	bne ReturnPokedexHeight
	mov r0, r5
	ldr r1, =0x81058E6 | 1
	bx r1

ReturnPokedexHeight:
	mov r4, r0 @Store height in r4
	mov r0, r5 @Store species in r0
	ldr r1, =SpeciesToNationalPokedexNum
	bl bxr1
	ldr r1, =0x81058F6 | 1
	bx r1

.pool
@0x8105A58 with r0
PokedexLoadAlternateWeightHook:
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r9, r3
	mov r5, r1 @Save species
	mov r0, r1
	mov r1, #0x1 @PKDX_GET_WEIGHT
	bl TryGetAlternateSpeciesSize
	cmp r0, #0x0
	bne ReturnPokedexWeight
	mov r0, r5
	ldr r1, =0x8105A60 | 1
	bx r1

ReturnPokedexWeight:
	mov r4, r0 @Store weight in r4
	mov r0, r5 @Store species in r0
	ldr r1, =SpeciesToNationalPokedexNum
	bl bxr1
	ldr r1, =0x8105A70 | 1
bxr1:
	bx r1

.pool
@0x8104ACA with r0
FixPokedexCheckNullSpeciesHook:
	cmp r3, #0x0 @Null species
	beq FixPokedexCheckNullSpeciesHook_ReturnNotSeenCaught
	sub r0, r3, #0x1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	lsr r0, r0, #0x13
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, =0x8104AD6 | 1
	bx r0

FixPokedexCheckNullSpeciesHook_ReturnNotSeenCaught:
	mov r0, #0x0
	ldr r1, =0x8104BB2 | 1
	bx r1

.pool
@0x80A0774 with r2
SizeMinigameLoadAlternateHeightHook:
	lsl r0, #0x10
	lsr r0, #0x10
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	mov r5, r0 @;Backup species
	mov r1, #0x0 @PKDX_GET_HEIGHT
	bl TryGetAlternateSpeciesSize
	cmp r0, #0x0
	bne ReturnAlternatWeightMinigame
	mov r0, r5
	ldr r1, =0x80A077E | 1
	bx r1

ReturnAlternatWeightMinigame:
	ldr r1, =0x80A078C | 1
	bx r1

.pool
@0x80146AC with r0
ForfeitBattleTowerHook:
	bl HandleRunActionFrontier
	cmp r0, #0x0
	bne ReturnPrintForfeitBattleTower
	ldr r0, =BATTLE_TYPE
	ldr r0, [r0]
	mov r1, #BATTLE_TRAINER | BATTLE_WIRELESS
	and r0, r1
	ldr r1, =0x80146B4 | 1
	bx r1

ReturnPrintForfeitBattleTower:
	ldr r0, =0x8014C4E | 1
	bx r0

.pool
@0x811F3C8 with r2
LoadMaxNumPokemonDisplaySelection:
	and r0, r1
	cmp r0, #0x80
	bne LoadMaxNumPokemonDisplaySelectionMax
	mov r3, #0x2
	b LoadMaxNumPokemonDisplaySelectionReturn

LoadMaxNumPokemonDisplaySelectionMax:
	mov r1, #0x0
	bl ChoosePokemon_LoadMaxPKMNStr
	mov r3, r0

LoadMaxNumPokemonDisplaySelectionReturn:
	ldr r0, =0x811F3D2 | 1
	bx r0

.pool
@0x8124184 with r0
LoadMaxNumPokemonChooseBattleTowerStringHook:
	mov r0, sp
	mov r1, #0x1
	bl ChoosePokemon_LoadMaxPKMNStr
	mov r8, r0
	ldr r0, =0x812418C | 1
	bx r0

.pool
@0x805BA30 with r0
AutoRunHook:
	mov r0, r5
	bl ShouldPlayerRun
	cmp r0, #0x0
	beq NoRunning
	ldr r0, =0x805BA5A | 1
	bx r0

NoRunning:
	ldr r0, =0x805BA8C | 1
	bx r0

.pool
@0x80BD338 with r0
BikeTurboBoostHook:
	mov r0, r4
	bl MoveOnBike
	ldr r0, =0x80BD350 | 1
	bx r0

.pool
@0x8046390 with r1
FlameBodyMagmaArmorEggHook:
	mov r1, r4
	bl SubtractEggSteps
	cmp r0, #0x0
	beq TimeToHatchEgg @A change implemented in Gen 5
	ldr r0, =0x804639E | 1
	bx r0

TimeToHatchEgg:
	mov r0, r6
	mov r1, r5
	bl TryDecrementingDaycareStepCounterIfMoreEggsToHatch
	ldr r0, =0x8046380 | 1
	bx r0

.pool
@0x80474F6 with r0
SkipEggHatchNicknameHook:
	bl ShouldSkipOfferEggHatchNickname
	cmp r0, #0x0
	beq SkipEggHatchNicknameReturn
	ldr r0, =sEggHatchData
	ldr r0, [r0]
	mov r1, #11 @;Fade out
	strb r1, [r0, #0x2]
	ldr r0, =0x8047684 | 1
	bx r0

SkipEggHatchNicknameReturn:
	ldr r0, =0x80474E8 | 1
	bx r0

.pool
@0x8046314 with r1
OvalCharmHook:
	bl ModifyBreedingScoreForOvalCharm
	mov r4, r0
	lsl r4, #0x18
	lsr r4, #0x18
	ldr r2, =Random
	bl bxr2
	ldr r1, =0x804631E | 1
	bx r1

.pool
@0x8045690 with r0
DaycareLevelCapHook:
	mov r1, r5
	add r1, #0x88
	ldr r1, [r1] @Steps
	mov r0, sp @Mon
	bl GetExperienceAfterDaycareSteps
	ldr r1, =0x8045698 | 1
	bx r1

.pool
@0x808BA60 with r2
SetBoxMonDataAtHook:
	mov r2, r5
	bl SetBoxMonDataAt
	pop {r4-r6,pc}

.pool
@0x808BB70 with r2
GetAndCopyBoxMonDataAtHook:
	mov r2, r5
	bl GetAndCopyBoxMonDataAt
	pop {r4-r6, pc}

.pool
@0x808BDF0 with r0
sub_808BDE8Hook:
	mov r0, r8
	bl sub_808BDE8
	pop {r3}
	mov r8, r3
	pop {r4-r7,pc}

.pool
@0x8093768 with r1
CompressedStorageSummaryScreenUpdate1:
	lsl r0, #0x18
	lsr r0, #0x18
	mov r1, #0x0
	bl GetCompressedMonPtr
	ldr r1, =0x8093772 | 1
	bx r1

.pool
@0x8138BD2 with r1
CompressedStorageSummaryScreenUpdate2:
	bl SummaryScreenBoxMonMultiplier
	add r0, r4
	mov r1, r5
	bl CompressedMonToMon
	pop {r4-r5,pc}


.pool
@0x8139208 with r1
SwapBoxMonMovesUpdate1:
	lsr r0, #0x18
	bl SummaryScreenBoxMonMultiplier
	add r6, r0
	ldr r0, =DAMAGE_LOC
	str r6, [r0] @Use the damage loc as temporary storage

	ldr r0, =BATTLE_DATA @Battle data is being used as a temporary mon
	mov r1, r6
	bl CreateBoxMonFromCompressedMon
	ldr r6, =BATTLE_DATA
	ldr r0, =0x8139212 | 1
	bx r0

.pool
@0x8139304 with r0
SwapBoxMonMovesUpdate2:
	ldr r0, =BATTLE_DATA
	ldr r1, =DAMAGE_LOC
	ldr r1, [r1]
	bl CreateCompressedMonFromBoxMon @Update the mon in the PC
	add sp, #0xC
	pop {r3-r5}
	mov r8, r3
	mov r9, r4
	mov r10, r5
	pop {r4-r7,pc}

.pool
@0x8093F58 with r3
ExpandedItemNameFixPokemonStorageSystemHook:
	bl FixItemNameInPokemonStorageSystem
	ldr r0, =0x8093F85 | 1
	bx r0

.pool
@0x80D860C with r0
ExpandedAbilityNamesBattle2:
	add r0, r5, r7
	ldrb r0, [r0, #0x1]
	ldr r1, =ABILITY_POPUP_SPECIES
	ldrh r1, [r1]
	bl GetAbilityName
	mov r1, r0
	ldr r0, =0x80D8618 | 1
	bx r0

.pool
@0x8136714 with r1
ExpandedAbilityNamesSummaryScreen:
	mov r1, r4 @;Ability
	push {r0-r1}
	ldr r0, [r6]
	add r0, r8
	mov r1, #11 @;MON_DATA_SPECIES
	ldr r2, =GetMonData
	bl bxr2
	mov r2, r0 @;Species
	pop {r0-r1}
	bl CopyAbilityName
	ldr r0, =0x8136720 | 1
	bx r0

.pool
@0x8136728 with r1
ExpandedAbilityDescriptionsSummaryScreen:
	mov r1, r4 @;Ability
	push {r0-r1}
	ldr r0, [r6]
	add r0, r8
	mov r1, #11 @;MON_DATA_SPECIES
	ldr r2, =GetMonData
	bl bxr2
	mov r2, r0 @;Species
	pop {r0-r1}
	bl CopyAbilityDescription
	ldr r0, =0x8136732 | 1
	bx r0

.pool
@0x8013144 with r0
LoadProperAbilityBattleDataHook:
	bl LoadProperAbilityBattleData
	ldr r0, =0x8013164 | 1
	bx r0

.pool
@0x8012850 with r0
BatonPassEffects:
	ldrb r1, [r3]
	lsl r1, #0x2
	add r1, r10
	ldr r0, [r1]
	ldr r2, =(STATUS3_SEEDED_RECOVERY | STATUS3_SEEDED | STATUS3_LOCKON | STATUS3_PERISH_SONG | STATUS3_ROOTED | STATUS3_LEVITATING | STATUS3_AQUA_RING | STATUS3_POWER_TRICK | STATUS3_ABILITY_SUPRESS)
	and r0, r2
	str r0, [r1]
BatonPassReturnReturn:
	ldr r0, =0x801285E | 1
	bx r0

.pool
@x80D8486 with r0
DisplayStatStageMessage:
	ldr r0, .SeverelyIndex
	cmp r0, r1
	beq BufferSeverely
	add r0, #0x1
	cmp r0, r1
	bne DisplayStatStageMessageNormal
	ldr r1, .DrasticallyStringPtr
	b DisplayStatStageMessageReturn

BufferSeverely:
	ldr r1, .SeverelyStringPtr
	b DisplayStatStageMessageReturn

DisplayStatStageMessageNormal:
	ldr r0, .gBattleStringsTable
	sub r1, #0xC
	lsl r1, r1, #0x2
	add r1, r1, r0
	ldr r1, [r1]

DisplayStatStageMessageReturn:
	ldr r0, =0x080D868E | 1
	bx r0

.align 2
.SeverelyIndex: .word 0x185
.gBattleStringsTable: .word 0x83FDF3C
.DrasticallyStringPtr: .word DrasticallyString
.SeverelyStringPtr: .word SeverelyString

.pool
@0x810BD64 with r1
CharacterCustomizationUpdateTrainerFrontPic2:
	mov r1, r4
	bl TryUpdateTrainerPicPalTrainerCard
	pop {r4-r5, pc}

.pool
@0x81090B8 with r0
SelectItemFromBagCheck:
	bl ShouldSelectItemFromBag
	cmp r0, #0x0
	bne SelectItemFromBagReturn
	ldr r1, =0x3005090 @;Tasks
	lsl r0, r6, #0x2
	add r0, r6
	lsl r0, #0x3
	ldr r2, =0x81090C0 | 1
	bx r2

SelectItemFromBagReturn:
	ldr r1, =0x8109024 | 1
	bx r1

@0x81323B0 with r0
SelectItemFromTMCaseCheck:
	bl ShouldSelectItemFromTMCase
	cmp r0, #0x0
	bne SelectItemFromTMCaseReturn
	ldr r0, =0x3005090 @;Tasks
	lsl r1, r7, #0x2
	add r1, r7
	lsl r1, #0x3
	ldr r2, =0x81323B8 | 1
bxr2:
	bx r2

SelectItemFromTMCaseReturn:
	ldr r0, =0x8132370 | 1
	bx r0

.pool
@0x807F542 with r0
WhiteoutScreenFix:
	mov r0, r1
	bl LoadProperWhiteoutString
	mov r1, r0
	mov r0, r6
	mov r2, #0x2
	mov r3, #0x8
	bl PrintWhiteoutTextScreen
	ldr r1, =0x807F54C | 1
	bx r1

PrintWhiteoutTextScreen:
	ldr r4, =0x807F3A4 | 1
	bx r4

.pool
@0x803401E with r4
TryHandleLaunchBattleTableAnimationHook:
	bl TryHandleLaunchBattleTableAnimation
	pop {r4-r7,pc}

.pool
@0x8077CD2 with r0
ReshowBattleScreenMonSpriteHook1:
	mov r0, r7
	bl IsInMiddleOfEndTurnSwitchIn
	cmp r0, #0x0
	bne ReshowBattleScreenNoSprite
	ldrh r0, [r6]
	mov r1, r10
	mul r0, r1
	add r0, r9
	ldr r1, =0x8077CDC | 1
	bx r1

.pool
@0x08077ECA with r0
ReshowBattleScreenMonSpriteHook2:
	mov r0, r7
	bl IsInMiddleOfEndTurnSwitchIn
	cmp r0, #0x0
	bne ReshowBattleScreenNoSprite
	ldrh r0, [r6]
	mov r1, r10
	mul r0, r1
	add r0, r9
	ldr r1, =0x8077ED4 | 1
	bx r1

ReshowBattleScreenNoSprite:
	mov r0, #0x0
	ldr r1, =0x8077FB4 | 1
	bx r1

.pool
@0x8078104 with r0
ReshowBattleScreenHealthboxSpriteHook:
	mov r0, r5
	bl IsInMiddleOfEndTurnSwitchIn
	cmp r0, #0x0
	bne ReshowBattleScreenNoHealthboxSprite
	mov r0, r5
	ldr r1, =GetBattlerSide
	bl bxr1
	lsl r0, #0x18
	ldr r1, =0x807810C | 1
	bx r1

ReshowBattleScreenNoHealthboxSprite:
	ldr r0, =0x8078128 | 1
	bx r0

.pool
@0x8022400 with r0
FrontierCheckBattleOverHook:
	ldr r0, =0x2023BC8 @BattleControllerExecFlags
	ldr r0, [r0]
	cmp r0, #0x0
	bne Atk24Return
	bl TryUpdateOutcomeForFrontierBattle
	cmp r0, #0x0
	bne Atk24Return @Is Frontier Battle
	ldr r0, =0x802240A | 1
	bx r0

Atk24Return:
	ldr r0, =0x802258A | 1
	bx r0

.pool
@0x8126E6C with r0
DracoMeteorMoveTutorHook:
	mov r0, r5
	bl TryHandleExcuseForDracoMeteorTutor
	cmp r0, #0x0
	beq LoadCantLearnMoveString

DracoMeteorSkipPrintString:
	mov r0, r6
	bl CancelPartyMenuLearnTutor
	ldr r0, =0x8126E98 | 1 @Print messages in overworld instead
	bx r0

LoadCantLearnMoveString:
	ldr r1, =0x8416DC2
	b PrintCantLearnMoveExcuseString

@0x8126E74 with r0
DracoMeteorMoveTutorHook2:
	mov r0, r5
	bl TryHandleExcuseForDracoMeteorTutorAlreadyKnow
	cmp r0, #0x0
	beq LoadAlreadyKnowsMoveString
	b DracoMeteorSkipPrintString

LoadAlreadyKnowsMoveString:
	ldr r1, =0x8416F10

PrintCantLearnMoveExcuseString:
	mov r0, r6
	ldr r2, =0x8125B14 | 1
	bl bxr2
	ldr r1, =0x8126E98 | 1
	bx r1

@0x8071F8E with r1
ModifyMegaCryHook:
	lsl r2, #0x18
	lsr r2, #0x18
	lsl r3, #0x18
	lsr r3, #0x18
	str r3, [sp]
	push {r0,r2}
	lsr r0, #0x10 @species
	mov r1, r4 @mode
	bl ModifyMegaCries
	mov r4, r0
	pop {r0,r2}
	ldr r3, =0x8071F98 | 1
	bx r3

@0x8136168 with r0
CamomonsSummaryScreenHook:
	ldr r5, .hword3290
	add r0, r2, r5
	push {r0}
	mov r1, #0xB @;Species
	ldr r2, =GetMonData
	bl bxr2
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r0, [r6]
	ldr r2, .hword3028
	add r0, r2
	mov r1, r4
	ldr r2, =GetSpeciesName
	bl bxr2
	bl ShouldReplaceTypesWithCamomons
	cmp r0, #0x0
	beq SummaryScreenLoadRegularTypes

SummaryScreenLoadCamomonsTypes:
	pop {r0}
	push {r0}
	mov r1, #0x0
	bl GetCamomonsTypeByMon
	ldr r1, [r6]
	ldr r3, .hword3220
	add r1, r3
	strb r0, [r1]
	pop {r0}
	mov r1, #0x1
	bl GetCamomonsTypeByMon
	ldr r1, [r6]
	ldr r3, .hword3220
	add r1, r3
	strb r0, [r1, #0x1]
	ldr r0, =0x81361A0 | 1
	bx r0

SummaryScreenLoadRegularTypes:
	pop {r0}
	ldr r0, =0x8136182 | 1
	bx r0

.align 2
.hword3220: .word 0x3220
.hword3290: .word 0x3290
.hword3028: .word 0x3028

.pool
@0x812461C with r0
FieldMoveBadgeHook:
	mov r0, r4
	bl HasBadgeToUseFieldMove
	cmp r0, #0x0
	beq NoBadgeForFieldMove
	ldr r0, =0x8124658 | 1
	bx r0

NoBadgeForFieldMove:
	ldr r0, =0x8124632 | 1
	bx r0

.pool
@0x806CB30 with r0
DiveCheckEmergeHook:
	mov r0, #0x80 @;B button pressed
	and r0, r1
	cmp r0, #0
	beq CheckPlayerMovement
	bl TrySetupDiveEmergeScript
	cmp r0, #0x0
	bne DiveChosen

CheckPlayerMovement:
	ldrb r1, [r5]
	mov r0, #0x40
	and r0, r1
	cmp r0, #0x0
	beq DidntTakeStepReturn
	ldr r0, =0x806CB38 | 1
	bx r0

DidntTakeStepReturn:
	ldr r0, =0x806CB74 | 1
	bx r0

DiveChosen:
	ldr r0, =0x806CB6C | 1
	bx r0

.pool
@0x806CCCC with r0
DiveCheckDownHook:
	ldrb r1, [r5]
	mov r0, #1 @;A button pressed
	and r0, r1
	cmp r0, #0
	beq CheckPlayerPressedStartButton
	bl TrySetupDiveDownScript
	cmp r0, #0x0
	bne DiveChosen

CheckPlayerPressedStartButton:
	ldrb r1, [r5]
	mov r0, #4 @;Start button pressed
	and r0, r1
	cmp r0, #0
	beq CheckSelectButtonReturn
	bl IsDexNavHudActive
	cmp r0, #0x0
	bne CheckSelectButtonReturn @No opening Start Menu while HUD is active
	ldr r2, =0x0806CCD6 | 1
	bx r2

CheckSelectButtonReturn:
	ldr r2, =0x0806CCFC | 1
	bx r2

.pool
@0x805BA1C with r0
DiveSpeedHook:
	mov r0, #0x8 @;Surfing
	and r0, r1
	cmp r0, #0x0
	bne MoveFasterOnWater
	bl IsUnderwater
	cmp r0, #0x0
	bne MoveFasterOnWater
	ldr r0, =0x805BA30 | 1
	bx r0

MoveFasterOnWater:
	mov r0, r4
	bl MovePlayerWhileSurfing
	ldr r0, =0x805BAA4 | 1
	bx r0

/*
	@For x4 movement speed
	mov r0, r4 @direction
	ldr r1, =PlayerGoSpeed4
	bl bxr1
	ldr r0, =0x805BAA4 | 1
	bx r0
*/

.pool
@0x80570D0 with r3
InitPlayerAvatarHook:
	ldr r3, =0x300500C @SaveBlock2
	ldr r3, [r3]
	ldrb r3, [r3, #0x8]
	bl InitPlayerAvatar
	ldrb r0, [r5]
	ldr r1, =0x80570D8 | 1
	bx r1

.pool
@0x809AC84 with r0
DNSCreateBuyMenuHook:
	ldr r0, =gInShop
	mov r1, #0x1
	strb r1, [r0]
	pop {r4, pc}

.pool
@0x809C078 with r1
DNSEndBuyMenuHook:
	ldr r1, =SetMainCallback2
	bl bxr1
	mov r0, r4
	ldr r1, =DestroyTask
	bl bxr1
	ldr r0, =gInShop
	mov r1, #0x0
	strb r1, [r0]
	ldr r0, =0x809C082 | 1
	bx r0

.pool
@0x804A00C with r4
HealthBarChangeAmountHook:
	mov r4, #0x6
	str r4, [sp]
	push {r0-r3}
	mov r0, r8
	bl CalcHealthBarPixelChange
	mov r4, r0
	pop {r0-r3}
	str r4, [sp, #0x4]
	ldr r4, =0x804A014 | 1
	bx r4

.pool
@0x804A066 with r1
ExpBarChangeAmountHook:
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r0, r8 @Bank
	bl ShouldFillExpBarToMax
	cmp r0, #0x0
	beq ExpBarNormalChangeAmount
	mov r0, #0x1
	neg r4, r0
	ldr r0, =0x804A080 | 1
	bx r0

ExpBarNormalChangeAmount:
	ldr r0, [r4, #0x4]
	ldr r1, [r4, #0x8]
	mov r3, r4
	ldr r2, =0x804A070 | 1
	bx r2

.pool
@0x81E381C with r0
ActivateMGBAPrint:
	add sp, #0x10
	bl mgba_open
	pop {r4, r7, pc}

.pool
@0x80C48AC with r0
CreateRoamerIconTownMapHook:
	bl CreateTownMapRoamerSprites
	pop {r4-r7, pc}

.pool
.align 2
@0x80C120C with r0
CreateRoamerIconTownMapPostSwitchMapHook:
	bl CreateTownMapRoamerSprites
	mov r0, #0x0 @Not invisible
	bl HideOrShowTownMapRoamerSprites
	ldr r1, [r5]
	ldr r0, =0x80001CC8
	lsl r0, #0x1
	lsr r0, #0x1 @Dumb compiler thing
	add r1, r0
	ldr r0, =0x80C1262 | 1
	bx r0

.pool
@0x80C111C with r0
DestroyRoamerIconTownMapSwitchHook:
	bl DestroyTownMapRoamerSprites
	ldr r1, [r4]
	mov r2, #0x80
	lsl r2, #0x5
	mov r0, #2
	ldr r3, =0x80C1124 | 1
	bx r3

.pool
@0x80C2D44 with r0
DestroyRoamerIconTownMapCloseHook:
	bl DestroyTownMapRoamerSprites
	ldr r1, =sMapOpenCloseAnim
	ldr r0, [r1]
	ldr r4, .CCE
	add r0, r4
	ldr r2, =0x080C2D4C | 1
	bx r2

.align 2
.CCE: .word 0xCCE

.pool
@0x80C49F8 with r0
HideOrShowRoamerIconTownMapHook:
	mov r0, r5
	bl HideOrShowTownMapRoamerSprites
	pop {r4-r7, pc}

.pool
@0x811E950 with r4
DrawLevelUpWindowPg2Hook:
	ldr r4, [sp, #0x4C] @Shadow Colour
	str r4, [sp] @Param Shadow Colour
	bl DrawLevelUpWindowPg2
	ldr r0, =0x811EA2E | 1
	bx r0

.pool
@0x8024C16 with r0
OpenPartyScreenBatonPassExplosionFix:
	cmp r1, #0x0
	beq OpenPartyScreenBatonPassExplosionFixReturn
	ldr r1, =IsDoubleBattle
	bl bxr1
	ldrb r3, [r4]
	cmp r0, #0x0 @Not doubles so no partner exists
	beq OpenPartyScreenBatonPassExplosionFixReturn
	mov r0, #0x2
	eor r3, r0
	strb r3, [r4]

OpenPartyScreenBatonPassExplosionFixReturn:
	ldr r0, =gActiveBattler
	ldrb r0, [r0]
	ldr r1, =gBattlersCount
	ldrb r1, [r1]
	cmp r0, r1
	bge OpenPartyScreenBatonPassExplosionFixNoStandbyMsg @Likely in Raid battles
	ldr r0, =0x8024C20 | 1
	bx r0

OpenPartyScreenBatonPassExplosionFixNoStandbyMsg:
	ldr r0, =0x8024C2E | 1
	bx r0

.pool
@0x800C61C with r0
MainMenuBadSaveTypeMsgHook:
	strh r6, [r1, #0x8]
	mov r0, r5
	ldr r1, =gText_ChangeSaveType
	bl PrintChangeSaveTypeErrorStatus
	pop {r4-r6, pc}

.pool
@0x0800C718 with r1
MainMenuRTCWarningHook1:
	mov r0, r4
	bl TryDisplayMainMenuRTCWarning
	cmp r0, #0x0
	bne MainMenuRTCWarningHook_WaitMsg
	mov r0, #0x40
	mov r1, #0x0
	ldr r2, =SetGpuReg
	bl bxr2
	mov r0, #0x44
	ldr r1, =0x800C720 | 1
	bx r1

MainMenuRTCWarningHook_WaitMsg:
	ldr r0, =0x800C774 | 1
	bx r0

.pool
@0x0800C7A4 with r1
MainMenuRTCWarningHook2:
	push {r7}
	sub sp, #0x10
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	bl TryDisplayMainMenuRTCWarning
	cmp r0, #0x0
	bne MainMenuRTCWarningHook_WaitMsg2
	ldr r0, =0x800C7AC | 1
	bx r0

MainMenuRTCWarningHook_WaitMsg2:
	ldr r0, =0x0800C9A2 | 1
	bx r0

.pool
@0x0805D524 with r0
SuctionCupsHook:
	bl DoesFishBite
	cmp r0, #0x0
	beq NoFishBiting
	ldr r0, =0x805D548 | 1
	bx r0

NoFishBiting:
	ldr r0, =0x805D53A | 1
	bx r0

.pool
@0x809D2FC with r3
RandomizerShowPokepicHook:
	lsl r1, r1, #0x18
	lsr r7, r1, #0x18
	lsl r2, r2, #0x18
	lsr r6, r2, #0x18
	mov r0, r8
	bl GetRandomizedSpecies
	mov r8, r0
	ldr r0, =0x809D304 | 1
	bx r0

.pool
@0x806C9AC with r0
FieldGetPlayerInputLButtonHook:
	mov r0, r5
	ldrh r1, [sp]
	bl FieldCheckIfPlayerPressedLButton
	ldr r0, =0x0806C9BA | 1
	bx r0

.pool
@0x806CCFC with r0
UseRegisteredItemHook:
	mov r0, r5
	bl ProcessNewFieldPlayerInput
	ldr r1, =0x0806CD22 | 1
	bx r1

.pool
@0x8041798 with r0
XSpDefStatBoostHook:
	ldr r0, [sp, #0x8] @Item
	mov r1, r3 @Boost amount
	bl NewXSpecialBoost
	mov r1, r0 @retVal
	ldr r0, =0x80417CA | 1
	bx r0

.pool
@0x8016650 with r0
AIXItemStringHook:
	bl PrepareStringForAIUsingXItem
	ldr r0, =gBattlescriptCurrInstr
	mov r9, r0
	ldr r0, =gCurrentActionFuncId
	mov r10, r0
	ldr r6, =0x81D99E4
	ldr r0, =0x8016706 | 1
	bx r0

.pool
@0x8136270 with 3
SummaryScreenOTGenderColour:
	add r2, #0x3
	ldr r3, =GetMonData
	bl bxr3
	ldr r0, [r6]
	add r0, r5
	mov r1, #0x31 @OT Gender
	ldr r3, =GetMonData
	bl bxr3
	mov r1, #0xFC
	mov r2, sp
	strb r1, [r2]
	mov r1, #0x1
	strb r1, [r2, #0x1]
	mov r1, #0x7
	cmp r0, #0x0
	beq GetOtGender_Cont
	mov r1, #0x1
GetOtGender_Cont:
	strb r1, [r2, #0x2]
	ldr r0, [r6]
	mov r4, #0xC1
	lsl r4, #0x6 @0x3040
	add r0, r4
	mov r1, sp
	mov r2, #10
	ldr r3, =0x8136280 | 1
	bx r3

.pool
@0x8137776 with r2
SummaryScreen_TradeMonMetLocationHook:
	ldr r2, =GetMonData
	bl bxr2
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	ldr r0, [r7]
	add r0, r4 @Mon
	mov r1, r5 @Met Location
	bl TryReplaceSummaryScreenLocationWithFarAwayLocation
	mov r5, r0
	ldr r1, =0x8137780 | 1
	bx r1

.pool
@0x8032CEC with r0
LastUsedBallOverrideHook:
	push {r4-r5, lr}
	sub sp, #0x4
	bl DidPlayerUseLastBallAndTryUpdateControllerFunc
	cmp r0, #0x0
	bne LastUsedBallOverrideHook_SkipBag
	mov r0, #0x1
	neg r0, r0
	ldr r1, =0x8032CF4 | 1
	bx r1

LastUsedBallOverrideHook_SkipBag: @Skips the palette fade to bag
	ldr r0, =0x8032D30 | 1
	bx r0

.pool
@0x8041C94 with r1
SitrusBerryOverworldEffectUpdate:
	ldr r1, [sp, #0x8] @Item Index
	cmp r1, #0x8E @Sitrus Berry
	bne NotSitrusBerry
	lsr r0, #0x1 @Reduce to 1/4

NotSitrusBerry:  
	cmp r0, #0x0
	bne SitrusBerryOverworldReturn
	mov r0, #0x1 @Restore at least 1 HP

SitrusBerryOverworldReturn:
	str r0, [sp] @HP to recover
	ldr r1, =0x8041CA6 | 1
	bx r1

.pool
@0x8140F44 with r1
SlotMachineExpandedCoinsDisplayFix:
	lsl r0, #0x10
	lsr r0, #0x10
	mov r8, r0

	mov r0, r9 @Coins
	ldr r1, .OldMaxCoins
	cmp r0, r1
	blt SkipCoinsDisplayFix
	mov r0, #0x0
	mov r9, r1 @Coins
	mov r8, r0 @0 Payout

SkipCoinsDisplayFix:
	mov r6, #0xFA
	lsl r6, #0x2
	ldr r0, =0x8140F4E | 1
	bx r0

.align 2
.OldMaxCoins: .word 9999

.pool
@0x8058E28 with r0
RemoveWalkThroughWallCheat:
	ldr r0, .3FF
	cmp r1, r0
	beq NotPassable
	mov r0, #0xC0
	lsl r0, #0x4
	ldr r2, =0x08058E32 | 1
	bx r2

NotPassable:
	mov r0, #0x1
	pop {r4-r7, pc}

.align 2
.3FF: .word 0x3FF

.pool
@0x809B18E with r0
@Swap order of mallocs so memory is moved around
RemoveShopModifierCheats:
	ldr r5, =sShopMenuItemStrings
	ldrh r0, [r4, #0x10]
	add r0, #0x1
	mov r1, #20 @Max item name length
	mul r0, r1
	ldr r1, =malloc
	bl bxr1
	str r0, [r5] @sShopMenuItemStrings
	cmp r0, #0x0
	beq FreeShopMemoryReturn

	ldrh r0, [r4, #0x10]
	add r0, #0x1
	lsl r0, #0x3
	ldr r1, =malloc
	bl bxr1
	str r0, [r7] @sShopMenuListMenu
	cmp r0, #0x0
	beq FreeShopMemoryReturn
	ldr r0, =0x809B1CC | 1
	bx r0

FreeShopMemoryReturn:
	ldr r0, =0x809B1B2 | 1
bxr0:
	bx r0

.pool
@0x8055880 with r0
LoadMapFromCameraTransition_DNSFixHook1:
	ldr r0, =gMapHeader
	ldr r0, [r0] @Map Layout (Footer)
	str r0, [sp] @Save for next hook

	@Call normal functions and return
	ldr r0, =Overworld_TryMapConnectionMusicTransition
	bl bxr0
	ldr r0, =ApplyCurrentWarp
	bl bxr0
	ldr r0, =0x8055888 | 1
	bx r0

.pool
@0x80558C4 with r0
LoadMapFromCameraTransition_DNSFixHook2:
	ldr r0, [sp] @Old map header - saved above in Hook1
	bl TryLoadTileset2OnCameraTransition
	ldr r0, =0x80558E2 | 1
	bx r0

/*
.pool
@0x80714D4 with r0
BlendPalettesOptimization:
	push {lr}
	lsl r1, #0x2
	bl BlendPalettesOptimized
	pop {pc}
*/

@0x8026330 with r0
AutoScrollBattleLevelUpBoxHook1:
	bl ShouldCloseBattleLevelUpBox
	cmp r0, #0x0
	beq AutoScrollBattleLevelUpBoxHook_ReturnNo
	ldr r0, =0x8026338 | 1
	bx r0

AutoScrollBattleLevelUpBoxHook_ReturnNo:
	ldr r0, =0x80263F6 | 1
	bx r0

@0x8026350 with r0
AutoScrollBattleLevelUpBoxHook2:
	bl ShouldCloseBattleLevelUpBox
	cmp r0, #0x0
	beq AutoScrollBattleLevelUpBoxHook_ReturnNo
	ldr r0, =0x8026358 | 1
	bx r0
