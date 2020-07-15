.text
.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global EventScript_SecondBagItemCanBeRegisteredToL
EventScript_SecondBagItemCanBeRegisteredToL:
	lockall
	msgbox gText_SecondBagItemCanBeRegisteredToL MSG_SIGN
	releaseall
	end

.global SystemScript_EnableAutoRun
SystemScript_EnableAutoRun:
	lockall
	checksound
	sound 0x2
	signmsg
	msgboxsign
	msgbox gText_AutoRunEnable MSG_SIGN
	checksound
	releaseall
	end
	
.global SystemScript_DisableAutoRun
SystemScript_DisableAutoRun:
	lockall
	checksound
	sound 0x3
	msgboxsign
	msgbox gText_AutoRunDisable MSG_SIGN
	checksound
	releaseall
	end

.global SystemScript_EnableBikeTurboBoost
SystemScript_EnableBikeTurboBoost:
	lockall
	@;checksound - otherwise lags after hopping on bike
	sound 0x2
	signmsg
	msgboxsign
	msgbox gText_BikeTurboBoostEnable MSG_SIGN
	checksound
	releaseall
	end
	
.global SystemScript_DisableBikeTurboBoost
SystemScript_DisableBikeTurboBoost:
	lockall
	@;checksound
	sound 0x3
	msgboxsign
	msgbox gText_BikeTurboBoostDisable MSG_SIGN
	checksound
	releaseall
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_PartyMenuFromField
SystemScript_PartyMenuFromField:
	lockall
	checksound
	sound 0x5 @SE_SELECT
	fadescreen FADEOUT_BLACK
	callasm InitPartyMenuFromField
	releaseall
	end

.global SystemScript_ItemMenuFromField
SystemScript_ItemMenuFromField:
	lockall
	checksound
	sound 0x5 @SE_SELECT
	fadescreen FADEOUT_BLACK
	callasm InitBagMenuFromField
	releaseall
	end

.global SystemScript_MiningScan
SystemScript_MiningScan:
	lockall
	checksound
	sound 0xCA @SE_TWINKLE
	getplayerpos 0x8000 0x8001
	setfieldeffectarg 0, 0x8000
	setfieldeffectarg 1, 0x8001
	setfieldeffectarg 2, TRUE
	dofieldeffect 48 @FLDEFF_THIN_RING
	callasm IsBestMiningSpotOutOfView
	compare LASTRESULT 0x0 @Out of view
	if notequal _goto SystemScript_MiningScan_SkipFieldEffect
	setfieldeffectarg 0, 0x8004
	setfieldeffectarg 1, 0x8005
	setfieldeffectarg 2, 0x8006
	dofieldeffect 54 @FLDEFF_SPARKLE
	waitfieldeffect 54 @FLDEFF_SPARKLE
	releaseall
	end

SystemScript_MiningScan_SkipFieldEffect:
	waitfieldeffect 48 @FLDEFF_THIN_RING
	releaseall
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_PoisonSurvial
SystemScript_PoisonSurvial:
	lockall
	msgboxsign
	msgbox gText_PoisonSurvial MSG_KEEPOPEN
	closeonkeypress
	releaseall
	end
	
@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.equ VAR_REPEL_STEP_COUNT, 0x4020

.global EventScript_BwRepelWoreOff
.global EventScript_RepelWoreOff
EventScript_BwRepelWoreOff:
	lockall
	checkitem 0x800E 1
	compare LASTRESULT 1
	if greaterorequal _goto AnotherRepel
	goto EventScript_RepelWoreOff
	
AnotherRepel:
	bufferitem 0x2 0x800E
	msgboxsign
	msgbox gText_AskUseAnotherRepel MSG_YESNO
	compare LASTRESULT 0x1
	if notequal _goto EndScript
	compare 0x800E ITEM_REPEL
	if equal _goto UseAnotherRepel
	compare 0x800E ITEM_SUPER_REPEL
	if equal _goto UseAnotherSuperRepel
	compare 0x800E ITEM_MAX_REPEL
	if equal _goto UseAnotherMaxRepel
	goto EndScript
	
UseAnotherRepel:
	removeitem ITEM_REPEL 1
	setvar VAR_REPEL_STEP_COUNT 100
	goto FinishNewRepel
	
UseAnotherSuperRepel:
	removeitem ITEM_SUPER_REPEL 1
	setvar VAR_REPEL_STEP_COUNT 200
	goto FinishNewRepel
	
UseAnotherMaxRepel:
	removeitem ITEM_MAX_REPEL 1
	setvar VAR_REPEL_STEP_COUNT 250
	goto FinishNewRepel
	
FinishNewRepel:
	checksound
	sound 0x29 @;Repel spray sound
	msgboxsign
	msgbox gText_UsedAnotherRepel MSG_SIGN
	goto EndScript
	
EventScript_RepelWoreOff:
	lockall
	msgboxsign
	msgbox gText_RepelWoreOff MSG_KEEPOPEN
	closeonkeypress
	
EndScript:
	releaseall
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_StartDexNavBattle
SystemScript_StartDexNavBattle:
	lockall
	call FollowerPositionFixScript
	checksound
	sound 0x15 @;Exclaim
	applymovement PLAYER PlayerExclaim
	waitmovement 0x0
	checksound
	pause 0x10
	dowildbattle
	releaseall
	end

PlayerExclaim:
.byte exclaim, end_m

.global SystemScript_DisplayDexnavMsg
SystemScript_DisplayDexnavMsg:
	lockall
	callstd MSG_NORMAL
	releaseall
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_WaitForFollower
SystemScript_WaitForFollower:
	lock
	pause 0x10
	release
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.equ SPECIAL_SHOW_ITEM_SPRITE_ON_FIND_OBTAIN, 0xE4
.equ SPECIAL_CLEAR_ITEM_SPRITE_AFTER_FIND_OBTAIN, 0xE5

.global SystemScript_FindItemMessage
SystemScript_FindItemMessage:
	textcolor BLACK
	hidesprite LASTTALKED
	pause 0x1
	special SPECIAL_SHOW_ITEM_SPRITE_ON_FIND_OBTAIN
	additem 0x8004 0x8005
	special2 LASTRESULT 0x196
	copyvar 0x8008 LASTRESULT
	compare 0x8008 0x1
	if equal _call 0x81A6821
	compare 0x8008 0x0
	if equal _call SystemScript_FindNormalItem
	waitfanfare
	waitmsg
	msgbox 0x81A5218 MSG_KEEPOPEN 
	special SPECIAL_CLEAR_ITEM_SPRITE_AFTER_FIND_OBTAIN
	return

SystemScript_FindNormalItem:
	compare 0x8005 0x1
	if greaterthan _goto SystemScript_FindMultipleNormalItems
	preparemsg 0x81A5231
	return

SystemScript_FindMultipleNormalItems:
	buffernumber 0x0 0x8005
	callasm TryAppendSOntoEndOfItemString
	preparemsg gText_FoundMultipleItems
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SystemScript_ObtainItem:
	copyvar 0x8013 0x8012
	copyvar 0x8004 0x8000 @;Copy item to 0x8004
	copyvar 0x8005 0x8001 @;Copy amount to 0x8005
	textcolor BLACK
	additem 0x8000 0x8001
	copyvar 0x8007 LASTRESULT
	call 0x81A6697
	copyvar 0x8012 0x8013
	return

.global SystemScript_ObtainItemMessage
SystemScript_ObtainItemMessage:
	special SPECIAL_SHOW_ITEM_SPRITE_ON_FIND_OBTAIN
	compare 0x8005 1
	if lessorequal _call ObtainedSingleItemMsg
	compare 0x8005 1
	if greaterthan _call ObtainedMultipleItemMsg
	waitfanfare
	waitmsg
	msgbox 0x81A5218 MSG_KEEPOPEN @;[PLAYER] put the item in the...
	setvar LASTRESULT 0x1
	special SPECIAL_CLEAR_ITEM_SPRITE_AFTER_FIND_OBTAIN
	return

ObtainedSingleItemMsg:
	special2 LASTRESULT 0x196
	compare LASTRESULT 0x0
	if equal _call ObtainedRegularItem
	compare LASTRESULT 0x1
	if equal _call ObtainedTMHM
	return

ObtainedTMHM:
	preparemsg gText_ObtainedTMHM
	return

ObtainedRegularItem:
	preparemsg 0x81A51F6 @;Obtained the item!
	return
	
ObtainedMultipleItemMsg:
	buffernumber 0x0 0x8005
	callasm TryAppendSOntoEndOfItemString
	preparemsg gText_ObtainedMultipleItems
	return

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.equ GAME_STAT_FOUND_HIDDEN_ITEM, 20

@Var8005 = Item
@Var8006 = Quantity
SystemScript_PickedUpHiddenItem: @;Replaces 81A6885
	callasm ShowItemSpriteOnFindHidden
	compare 0x8006 1
	if equal _call 0x81A68AA @;EventScript_FoundSingleHiddenItem
	compare 0x8006 1
	if notequal _call 0x81A68BA @;EventScript_FoundMultipleHiddenItems
	waitfanfare
	waitmsg
	msgbox 0x81A5218 MSG_KEEPOPEN @;gText_PutItemAway
	callasm ClearItemSpriteAfterFindHidden
	special 0x96 @;SetHiddenItemFlag
	incrementgamestat GAME_STAT_FOUND_HIDDEN_ITEM
	releaseall
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.macro showselectitems num
EventScript_ShowSelectItems\num:
	multichoice 0x0 0x0 TWO_MULTICHOICE_OPTIONS + \num - 2 0x0
	goto EventScript_ChooseSelectItem
.endm

.global EventScript_ShowSelectItems
EventScript_ShowSelectItems:
	lockall
	preparemsg gText_UseWhichRegisteredItem
	waitmsg
	switch 0x8004
	case 2, EventScript_ShowSelectItems2
	case 3, EventScript_ShowSelectItems3
	case 4, EventScript_ShowSelectItems4
	case 5, EventScript_ShowSelectItems5
	case 6, EventScript_ShowSelectItems6
	releaseall
	end

showselectitems 2
showselectitems 3
showselectitems 4
showselectitems 5
showselectitems 6

EventScript_ChooseSelectItem:
	comparevars LASTRESULT 0x8004
	if greaterorequal _goto .LEnd @Chose to cancel
	callasm UseChosenRegisteredItem
.LEnd:
	releaseall
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.equ FLAG_AUTO_HMS, 0x902 @For Unbound
.equ SPECIAL_DISMOUNT_BICYCLE, 0xAF
.global EventScript_UseRockClimb
EventScript_UseRockClimb:
	bufferpartypokemon 0x0 0x8004
	bufferattack 0x1 MOVE_ROCKCLIMB
	callasm IsUnboundToVar
	compare LASTRESULT 0x0
	if equal _goto EventScript_UseRockClimb_Ask
	checkflag FLAG_AUTO_HMS
	if SET _goto EventScript_UseRockClimb_SkipAsk

EventScript_UseRockClimb_Ask:
	msgbox gText_WantToScaleCliff MSG_YESNO
	compare LASTRESULT NO
	if equal _goto EventScript_RockClimbEnd
	msgbox 0x81BDFD7 MSG_NORMAL @;[BUFFER1] used [BUFFER2]!

EventScript_UseRockClimb_SkipAsk:
	setanimation 0x0 0x8004
	goto EventScript_RockClimb

.global EventScript_JustRockWall
EventScript_JustRockWall:
	msgbox gText_RockIsRocky MSG_NORMAL
EventScript_RockClimbEnd:
	closeonkeypress
	end

.equ SPECIAL_GET_PLAYER_FACING, 0x1AA
.global EventScript_RockClimb
EventScript_RockClimb:
	lockall
	call FollowerIntoPlayerScript
	callasm HideFollower
	special SPECIAL_DISMOUNT_BICYCLE
	doanimation 0x25
	waitstate
	callasm StopPlayerMotion
	callasm sp09A_StopSounds
	special2 PLAYERFACING SPECIAL_GET_PLAYER_FACING
	compare PLAYERFACING DOWN
	if equal _goto EventScript_RockClimbDown
	compare PLAYERFACING LEFT
	if equal _goto EventScript_RockClimbLeft
	compare PLAYERFACING RIGHT
	if equal _goto EventScript_RockClimbRight

EventScript_RockClimbUp:
	sound 0x7C @Rock Smash
	applymovement PLAYER m_RockClimbUp
	waitmovement PLAYER
	callasm ShouldRockClimbContinue
	compare LASTRESULT 0
	if equal _goto EventScript_RockClimbUpFinish
	goto EventScript_RockClimbUp

EventScript_RockClimbDown:
	sound 0x7C @Rock Smash
	applymovement PLAYER m_RockClimbDown
	waitmovement PLAYER
	callasm ShouldRockClimbContinue
	compare LASTRESULT 0
	if equal _goto EventScript_RockClimbDownFinish
	goto EventScript_RockClimbDown

EventScript_RockClimbLeft:
	sound 0x7C @Rock Smash
	applymovement PLAYER m_RockClimbLeft
EventScript_RockClimbLeftWaitmovement:
	waitmovement PLAYER
	callasm ShouldRockClimbContinueDiagonally
	compare LASTRESULT 1
	if equal _goto EventScript_RockClimbLeftUp
	compare LASTRESULT 2
	if equal _goto EventScript_RockClimbLeftDown
	callasm ShouldRockClimbContinue
	compare LASTRESULT 0
	if equal _goto EventScript_RockClimbLeftFinish
	goto EventScript_RockClimbLeft
	
EventScript_RockClimbLeftUp:
	sound 0x7C @Rock Smash
	applymovement PLAYER m_RockClimbLeftUp
	goto EventScript_RockClimbLeftWaitmovement

EventScript_RockClimbLeftDown:
	sound 0x7C @Rock Smash
	applymovement PLAYER m_RockClimbLeftDown
	goto EventScript_RockClimbLeftWaitmovement

EventScript_RockClimbRight:
	sound 0x7C @Rock Smash
	applymovement PLAYER m_RockClimbRight
EventScript_RockClimbRightWaitmovement:
	waitmovement PLAYER
	callasm ShouldRockClimbContinueDiagonally
	compare LASTRESULT 1
	if equal _goto EventScript_RockClimbRightUp
	compare LASTRESULT 2
	if equal _goto EventScript_RockClimbRightDown
	callasm ShouldRockClimbContinue
	compare LASTRESULT 0
	if equal _goto EventScript_RockClimbRightFinish
	goto EventScript_RockClimbRight
	
EventScript_RockClimbRightUp:
	sound 0x7C @Rock Smash
	applymovement PLAYER m_RockClimbRightUp
	goto EventScript_RockClimbRightWaitmovement

EventScript_RockClimbRightDown:
	sound 0x7C @Rock Smash
	applymovement PLAYER m_RockClimbRightDown
	goto EventScript_RockClimbRightWaitmovement

EventScript_RockClimbDownFinish:
	applymovement PLAYER m_RockClimbDown
	goto EventScript_RockClimbFinish

EventScript_RockClimbUpFinish:
	applymovement PLAYER m_RockClimbUp
	goto EventScript_RockClimbFinish

EventScript_RockClimbLeftFinish:
	applymovement PLAYER m_RockClimbLeft
	goto EventScript_RockClimbFinish

EventScript_RockClimbRightFinish:
	applymovement PLAYER m_RockClimbRight
	
EventScript_RockClimbFinish:
	waitmovement PLAYER
	callasm StartPlayerMotion
	callasm FollowMe_WarpSetEnd
	releaseall
	end

m_RockClimbDown: .byte run_down, end_m
m_RockClimbUp: .byte run_up, end_m
m_RockClimbLeft: .byte run_left, end_m
m_RockClimbRight: .byte run_right, end_m

m_RockClimbLeftUp: .byte 0xE0, end_m
m_RockClimbLeftDown: .byte 0xDC, end_m
m_RockClimbRightUp: .byte 0xE1, end_m
m_RockClimbRightDown: .byte 0xDD, end_m

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global EventScript_Defog
EventScript_Defog:
	lockall
	doanimation 0x28
	waitstate
	checksound
	sound 0x7E
	setweather 0x0
	doweather
	checksound
	msgbox gText_DefogBlewAwayFog MSG_KEEPOPEN
	releaseall
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.equ SPECIAL_POKEMON_TYPE_IN_PARTY, 0xB2
.global EventScript_UseLavaSurf
.global EventScript_UseLavaSurf_Debug
EventScript_UseLavaSurf:
	setvar 0x8000 TYPE_FIRE
	special SPECIAL_POKEMON_TYPE_IN_PARTY
	compare LASTRESULT PARTY_SIZE
	if equal _goto EventScript_MagmaGlistens
	copyvar 0x8004 LASTRESULT
EventScript_UseLavaSurf_Debug:
	bufferpartypokemon 0x0 0x8004
	callasm IsUnboundToVar
	compare LASTRESULT 0x0
	if equal _goto EventScript_UseLavaSurf_Ask
	checkflag FLAG_AUTO_HMS
	if SET _goto EventScript_UseLavaSurf_SkipAsk

EventScript_UseLavaSurf_Ask:
	msgbox gText_LikeToLavaSurf MSG_YESNO
	compare LASTRESULT NO
	if equal _goto EventScript_LavaSurfEnd
	lockall
	msgbox gText_LavaSurfedWith MSG_KEEPOPEN

EventScript_UseLavaSurf_SkipAsk:
	lockall
	setanimation 0x0 0x8004
	doanimation 0x9
	releaseall
	end

.global EventScript_MagmaGlistens
EventScript_MagmaGlistens:
	msgbox gText_LavaDyed MSG_NORMAL
EventScript_LavaSurfEnd:
	closeonkeypress
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global EventScript_UseWaterfall
EventScript_UseWaterfall:
	bufferpartypokemon 0x0 0x8004
	bufferattack 0x1 MOVE_WATERFALL
	callasm IsUnboundToVar
	compare LASTRESULT 0x0
	if equal _goto EventScript_UseWaterfall_Ask
	checkflag FLAG_AUTO_HMS
	if SET _goto EventScript_UseWaterfall_SkipAsk

EventScript_UseWaterfall_Ask:
	msgbox 0x81BE33F MSG_YESNO
	compare LASTRESULT NO
	if equal _goto EventScript_WaterfallEnd
	lockall
	call FollowerIntoPlayerScript
	callasm HideFollower
	msgbox 0x81BDFD7 MSG_KEEPOPEN

EventScript_UseWaterfall_SkipMsg:
	setanimation 0x0 0x8004
	doanimation 0x2B
	callasm FollowMe_WarpSetEnd
EventScript_WaterfallEnd:
	releaseall
	end

EventScript_UseWaterfall_SkipAsk:
	lockall
	call FollowerIntoPlayerScript
	callasm HideFollower
	goto EventScript_UseWaterfall_SkipMsg

.global EventScript_WallOfWater
EventScript_WallOfWater:
	msgbox 0x81BE30A MSG_NORMAL
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global EventScript_UseSurf
EventScript_UseSurf:
	bufferpartypokemon 0x0 0x8004
	bufferattack 0x1 MOVE_SURF
	callasm IsUnboundToVar
	compare LASTRESULT 0x0
	if equal _goto EventScript_UseSurf_Ask
	checkflag FLAG_AUTO_HMS
	if SET _goto EventScript_UseSurf_SkipAsk

EventScript_UseSurf_Ask:
	callasm IsCurrentAreaSwampToVar
	compare LASTRESULT 0x0
	if notequal _goto EventScript_UseSurf_AskMurkyWater
	msgbox 0x81A556E MSG_YESNO	
EventScript_UseSurf_CheckAnswer:
	compare LASTRESULT NO
	if equal _goto EventScript_SurfEnd
	lockall
	msgbox 0x81BDFD7 MSG_KEEPOPEN

EventScript_UseSurf_SkipAsk:
	lockall
	setanimation 0x0 0x8004
	doanimation 0x9
EventScript_SurfEnd:
	releaseall
	end

EventScript_UseSurf_AskMurkyWater:
	msgbox gText_WaterMurkyBrownUseSurf MSG_YESNO	
	goto EventScript_UseSurf_CheckAnswer

.global EventScript_WaterDyedBlue
EventScript_WaterDyedBlue:
	msgbox gText_WaterDyed MSG_NORMAL
	end

.global EventScript_WaterMurkyBrown
EventScript_WaterMurkyBrown:
	msgbox gText_WaterMurkyBrown MSG_NORMAL
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.equ FLDEFF_USE_DIVE, 44
.global EventScript_UseDive
EventScript_UseDive:
	bufferpartypokemon 0x0 0x8004
	bufferattack 0x1 MOVE_DIVE
	callasm IsUnboundToVar
	compare LASTRESULT 0x0
	if equal _goto EventScript_UseDive_Ask
	checkflag FLAG_AUTO_HMS
	if SET _goto EventScript_UseDive_SkipAsk

EventScript_UseDive_Ask:
	msgbox gText_WantToDive MSG_YESNO
	compare LASTRESULT NO
	if equal _goto EventScript_EndDive
	lockall
	call FollowerIntoPlayerScript
	callasm HideFollower
	msgbox 0x81BDFD7 MSG_NORMAL
	setanimation 0x0 0x8004
	setanimation 0x1 1
	doanimation FLDEFF_USE_DIVE
	goto EventScript_EndDive

.global EventScript_CantDive
EventScript_CantDive:
	msgbox gText_CantDive MSG_NORMAL
EventScript_EndDive:
	releaseall
	end

EventScript_UseDive_SkipAsk:
	lockall
	call FollowerIntoPlayerScript
	callasm HideFollower
	setanimation 0x0 0x8004
	setanimation 0x1 1
	doanimation FLDEFF_USE_DIVE
	goto EventScript_EndDive	

.global EventScript_UseDiveUnderwater
EventScript_UseDiveUnderwater:
	bufferpartypokemon 0x0 0x8004
	bufferattack 0x1 MOVE_DIVE
	msgbox gText_WantToSurface MSG_YESNO
	compare LASTRESULT NO
	if equal _goto EventScript_EndSurface
	lockall
	call FollowerIntoPlayerScript
	callasm HideFollower
	msgbox 0x81BDFD7 MSG_NORMAL
	setanimation 0x0 0x8004
	setanimation 0x1 1
	doanimation FLDEFF_USE_DIVE
	callasm FollowMe_SetIndicatorToRecreateSurfBlob
	goto EventScript_EndSurface
	end

.global EventScript_CantSurface
EventScript_CantSurface:
	msgbox gText_CantSurface MSG_NORMAL
EventScript_EndSurface:
	releaseall
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.equ FLAG_SYS_USE_FLASH, 0x806
.global EventScript_UseFlash
EventScript_UseFlash:
	lockall
	bufferpartypokemon 0x0 0x8004
	bufferattack 0x1 MOVE_FLASH
	setflag FLAG_SYS_USE_FLASH
	msgbox 0x81BDFD7 MSG_NORMAL
	checksound
	sound 0xC8
	animateflash 0x0
	setflashradius 0x0
	releaseall
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global EventScript_LockedDoor
EventScript_LockedDoor:
	msgbox gText_DoorIsLocked MSG_SIGN
	end

.global EventScript_PsychicBarrier
EventScript_PsychicBarrier:
	msgbox gText_PsychicBarrier MSG_SIGN
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global EventScript_Ladder
EventScript_Ladder:
	special SPECIAL_GET_PLAYER_FACING
	compare PLAYERFACING LEFT
	if equal _goto EventScript_FacingLadderSideways
	compare PLAYERFACING RIGHT
	if equal _goto EventScript_FacingLadderSideways
	msgbox gText_ClimbLadder MSG_YESNO
	compare LASTRESULT NO
	if equal _goto EventScript_LadderEnd
	closeonkeypress
	lockall
	call FollowerIntoPlayerScript
	callasm HideFollower
	special SPECIAL_DISMOUNT_BICYCLE
	compare PLAYERFACING DOWN
	if equal _goto EventScript_LadderDown

EventScript_LadderUp:
	applymovement PLAYER m_LadderClimbUp
	waitmovement 0x0
	setvar PLAYERFACING UP
	callasm ShouldLadderClimbContinue
	compare LASTRESULT 0
	if equal _goto EventScript_LadderUpFinish
	goto EventScript_LadderUp

EventScript_LadderDown:
	applymovement PLAYER m_LadderClimbDown
	waitmovement 0x0
	setvar PLAYERFACING DOWN
	callasm ShouldLadderClimbContinue
	compare LASTRESULT 0
	if equal _goto EventScript_LadderDownFinish
	goto EventScript_LadderDown

EventScript_LadderUpFinish:
	applymovement PLAYER m_LadderClimbUp
	goto EventScript_RockClimbFinish

EventScript_LadderDownFinish:
	applymovement PLAYER m_LadderClimbDown
	goto EventScript_RockClimbFinish

EventScript_FacingLadderSideways:
	msgbox gText_CantReachLadder MSG_NORMAL
EventScript_LadderEnd:
	releaseall
	end

m_LadderClimbUp: .byte walk_up_slow, end_m
m_LadderClimbDown: .byte 0xAF, end_m

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global EventScript_HiddenGrottoForest
EventScript_HiddenGrottoForest:
	msgboxsign
	msgbox gText_FollowNarrowPath MSG_YESNO
	compare LASTRESULT NO
	if equal _goto EventScript_HiddenGrottoForest_Reject
	release
	callasm FollowHiddenGrottoWarp
	compare LASTRESULT 0x0
	if equal _goto EventScript_HiddenGrottoForest_Nowhere
	waitstate

EventScript_HiddenGrottoForest_Reject:
	msgboxnormal
	release
	end
	
EventScript_HiddenGrottoForest_Nowhere:
	msgbox gText_FollowNarrowPathNowhere MSG_NORMAL
	msgboxnormal
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.equ SE_HEADBUTT, 0x74

.global EventScript_HeadbuttTree
EventScript_HeadbuttTree:
	checkattack MOVE_HEADBUTT
	compare LASTRESULT PARTY_SIZE
	if equal _goto EventScript_HeadbuttTree_NoUsableMons
	setanimation 0x0 LASTRESULT
	bufferpartypokemon 0x0 LASTRESULT
	bufferattack 0x1 MOVE_HEADBUTT
	checkflag FLAG_AUTO_HMS
	if SET _goto EventScript_HeadbuttTree_SkipAsk
	msgbox gText_WantToUseHeadbutt MSG_YESNO
	compare LASTRESULT NO
	if equal _goto EventScript_HeadbuttTree_End

EventScript_HeadbuttTree_SkipAsk:
	lockall
	msgbox 0x81BDFD7 MSG_NORMAL
	doanimation 0x28
	waitstate
	checksound
	sound SE_HEADBUTT
	setvar 0x8004 0x5 @This controls how far the screen shakes vertically
	setvar 0x8005 0x0 @This controls how far the screen shakes horizontally
	setvar 0x8006 0x8 @This controls how long the overall animation lasts
	setvar 0x8007 0x3 @This controls how long one screen shake lasts
	special 0x136 @SPECIAL_SHAKE_SCREEN
	waitstate
	special 0xAB @Rock Smash wild mon special
	compare LASTRESULT 0x0 @No data for this area
	if equal _goto EventScript_HeadbuttTree_End
	waitstate

EventScript_HeadbuttTree_End:
	releaseall
	end

EventScript_HeadbuttTree_NoUsableMons:
	msgbox gText_TreeCanBeHeadbutted MSG_NORMAL
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global EventScript_UndergroundMining
EventScript_UndergroundMining:
	msgbox gText_StartMining MSG_YESNO
	compare LASTRESULT NO
	if equal _goto EventScript_UndergroundMining_End
	callasm ResetMiningSpots
	startmining @Right now is not implemented
	waitstate
	callasm PrepMiningWarp @Try to go through door if there is one
	compare LASTRESULT 0x0
	if equal _goto EventScript_UndergroundMining_End

	@Update door tiles
	sound 0x7C @Rock Smash
	getplayerpos 0x8004 0x8005
	subvar 0x8004 1 @1 Left
	subvar 0x8005 2 @2 Up
	setmaptile 0x8004 0x8005 0x2D0 0x1
	addvar 0x8004 1 @1 Right
	setmaptile 0x8004 0x8005 0x2D1 0x1
	addvar 0x8004 1 @1 Right
	setmaptile 0x8004 0x8005 0x2D2 0x1
	subvar 0x8004 2 @2 Left
	addvar 0x8005 1 @1 Down
	setmaptile 0x8004 0x8005 0x2D8 0x1
	addvar 0x8004 1 @1 Right
	setmaptile 0x8004 0x8005 0x2D9 0x1
	addvar 0x8004 1 @1 Right
	setmaptile 0x8004 0x8005 0x2DA 0x1
	subvar 0x8004 2 @2 Left
	addvar 0x8005 1 @1 Down
	setmaptile 0x8004 0x8005 0x2E0 0x0
	addvar 0x8004 1 @1 Right
	setmaptile 0x8004 0x8005 0x2E1 0x0
	addvar 0x8004 1 @1 Right
	setmaptile 0x8004 0x8005 0x2E2 0x0
	special 0x8E @Reload tiles
	checksound
	applymovement PLAYER m_WalkUp1
	waitmovement 0x0
	callasm DoWarp
	waitstate

EventScript_UndergroundMining_End:
	release
	end

m_WalkUp1: .byte walk_up, end_m

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_StopZooming
SystemScript_StopZooming:
	lockall
	msgbox gText_StopZooming MSG_KEEPOPEN
	pause 0x80
	releaseall
	end

@;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global SystemScript_DebugMenu
SystemScript_DebugMenu:
	lockall
	multichoiceoption gText_DebugMenu_SetFlag 0
	multichoiceoption gText_DebugMenu_GiveItem 1
	multichoiceoption gText_DebugMenu_Level100Team 2
	multichoiceoption gText_DebugMenu_MaxCoinage 3
	multichoice 0x0 0x0 FOUR_MULTICHOICE_OPTIONS 0x0
	switch LASTRESULT
	case 0, SystemScript_DebugMenu_SetFlag
	case 1, SystemScript_DebugMenu_GiveItem
	case 2, SystemScript_DebugMenu_Level100Team
	case 3, SystemScript_DebugMenu_MaxCoinage
SystemScript_DebugMenu_End:
	releaseall
	end

SystemScript_DebugMenu_SetFlag:
	multichoiceoption gText_DebugMenu_AllBadges 0
	multichoiceoption gText_DebugMenu_GameClear 1
	multichoiceoption gText_DebugMenu_Pokedexes 2
	multichoiceoption gText_DebugMenu_FlySpots 3
	multichoiceoption gText_DebugMenu_CustomFlag 4
	multichoice 0x0 0x0 FIVE_MULTICHOICE_OPTIONS 0x0
	compare LASTRESULT 0x5
	if greaterorequal _goto SystemScript_DebugMenu_End
	callasm DebugMenu_ProcessSetFlag
	goto SystemScript_DebugMenu_SetFlag

SystemScript_DebugMenu_GiveItem:
	multichoiceoption gText_DebugMenu_UsefulKeyItems 0
	multichoiceoption gText_DebugMenu_GeneralUsefulItems 1
	multichoiceoption gText_DebugMenu_PokeBalls 2
	multichoiceoption gText_DebugMenu_Berries 3
	multichoiceoption gText_DebugMenu_TMs 4
	multichoiceoption gText_DebugMenu_AllItems 5
	multichoice 0x0 0x0 SIX_MULTICHOICE_OPTIONS 0x0
	compare LASTRESULT 0x6
	if greaterorequal _goto SystemScript_DebugMenu_End
	callasm DebugMenu_ProcessGiveItem
	goto SystemScript_DebugMenu_GiveItem

SystemScript_DebugMenu_Level100Team:
	callasm DebugMenu_SetTeamToLevel100
	goto SystemScript_DebugMenu

SystemScript_DebugMenu_MaxCoinage:
	callasm DebugMenu_MaxMoneyAndCoins
	goto SystemScript_DebugMenu
