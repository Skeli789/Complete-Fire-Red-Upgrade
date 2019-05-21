.text
.align 2
.thumb
.thumb_func
/*
script functions/specials in asm - hooks and returns

*/

.include "../asm_defines.s"

@@ Specials (credit to JPAN)
.global sp097_WildGroundBattle
.global sp098_WildSeaBattle
.global sp156_GhostBattleSpecial

@@ Hidden Abilities (credit to azurile13)
.global DaycareInheritHiddenAbility
@ see special_inserts for most of the code

@@ Character Customization (credit to JPAN)
.global NpcSpawnWithTemplate
.global NpcSizeFix
.global NewNpcInfo
.global LinkNpcFix
.global NpcOffscreenFix
.global NpcWaterFix
.global CreateSpriteLoadTable	@credit to ghoulslash

@@ Dynamic Overworld Palettes (credit to Navenatox)
.global SetPalFossilImage
.global LoadPalFossilImage
.global DeleteOBJFreeTiles
.global DeleteAllOBJs
.global DeleteReflection
.global DeleteWarpArrow
.global SetPalMisc
.global SetPalEmotionBubble
.global SetPalWarpArrow
.global SetPalItemfinderArrow
.global SetPalFameCheckerUnknown
.global SetPalNPC1
.global SetPalNPC2
.global SetPalNPC3
.global SetPalNPC4
.global SetPalReflection
.global SetPalSurf
.global SetPalFly
.global SetPalFly2
.global SetPalShadow
.global RefreshFameCheckerNPCs
.global SetPalSSAnneWake
.global SetPalSSAnneSmoke
.global GetDarkeningType1
.global GetDarkeningType2
.global GetDarkeningType3
.global FogBrightenAll
.global FogBrightenOnStep
.global FogBrightenAndFadeIn
.global GetFadeType1
.global GetFadeType2

@@ Dex Nav (Credit to FBI)
.global CheckDexNavSelect
.global PokeToolNoFade

@@ Whiteout Hack
.global DynamicWhiteoutMap

@@ Wrap-around Summary Screen (credit to Sagiri)
.global SummaryScreenWrapAround

@@ Direct PC Selection (credit to ghoulslash)
.global pcSelect_ClosingCheck
.global pcSelect_HackWithdrawBRM
.global pcSelect_ForceMulti
.global pcSelect_ChooseSelect
.global pcSelect_newBrmTable
.global pcSelect_NoPartyString
.global pcSelect_OperationsMsg
.global pcSelect_OperationsTable
.global pcSelect_SkipMultiGen
.global pcSelect_SkipWithdrawCount
.global pcSelect_SwapDecision

@@ Unhidden Power (credit to Sagiri)
.global WriteTypeHook
.global DisplayTypeHook

@@ Evolution Method Extra Hooks
.global RemoveEvoItem

@@ Tm/Hm Expansion
.global SortTmHms
.global FixTmHmDiscLoader
.global FixTmHmDiscLoader2
.global FixTmHmDiscPal
.global FixTmHmDiscPos
.global HmSymbolFix
.global SelectedTmSymbolFix
.global FixMartTmListing
.global TmMartDescriptionFix

@@ Reusable TMs
.global ReusableTMCheck1
.global ReusableTMCheck2
.global ReusableTMCheck3
.global SingleTmPurchaseFix
.global AddSingleTmFix
.global UnbuyableTmFix

@@ Start Menu Stuff
.global FixStartMenuSize

@@ Physical Special Split Icons
.global PhysicalSpecialSplitIconRoutine

@@ Move Reminder
.global FixReminderCalloc
.global FixNumMovesLimiter
.global FixMultiUseTemplate
.global FixLoadMoveId1
.global FixLoadMoveId2
.global FixLoadMoveId3
.global FixLoadMoveId4
.global FixLoadMoveId5
.global FixMoveNameLoading
.global FixMoveReminderDataLoading

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - fix move to load data from
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixMoveReminderDataLoading:
	bl GetMoveIdFromRelearnerStruct
	ldr r1, =(0x080E54D8 +1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Fix Move Name Loading
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixMoveNameLoading:
	mov r0, r3	@cursor pos
	bl CopyMoveReminderMoveName
	ldr r1, =(0x080E5250 +1)
	bx r1
	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Fix relearnable moves counter
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixNumRelearnableMoves:
	push {r4-r7}
	bl GetRelearnableMoves
	pop {r4-r7}
	mov r12, r0		@total list num count
	mov r8, r0
	ldr r2, =(0x080E51AA +1)
	bx r2

/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - fix multi use template
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixMultiUseTemplate:
	bl SetMoveReminderMultiTemplate
	ldr r3, =(0x080E51D6 +1)
	bx r3
*/

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder Calloc
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ E47A4 via r0
FixReminderCalloc:
	bl InitLearnMoveFix
	ldr r0, =(0x080E47AC +1)
	bx r0
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Move Reminder - Fix Move Id Loading
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixLoadMoveId1:
	push {r0, r2-r7}
	ldrb r0, [r1]	@cursor pos
	bl GetMoveIdFromRelearnerStruct
	mov r1, r0
	pop {r0, r2-r7}
	ldr r2, =(0x080E4AAA +1)
	bx r2	

.align 2
.pool
FixLoadMoveId2:
	push {r0-r3, r5-r7}
	ldrb r0, [r4]	@ cursor pos
	bl GetMoveIdFromRelearnerStruct
	mov r4, r0
	pop {r0-r3,r5-r7}
	ldr r6, =(0x080E4CBC +1)
	bx r6

.align 2
.pool
FixLoadMoveId3:
	push {r0, r3-r7}	@gMoveRelearnerStruct
	mov r0, r2
	bl GetMoveIdFromRelearnerStruct
	mov r1, r0
	pop {r0, r3-r7}
	ldr r2, =(0x080E4E14 +1)
	bx r2

.align 2
.pool
FixLoadMoveId4:
	mov r0, r1
	bl GetMoveIdFromRelearnerStruct
	mov r1, r0
	ldr r2, =(0x080E4E2E +1)
	bx r2

.align 2
.pool
FixLoadMoveId5:
	bl InitMoveRelearnerMoveIDs
	ldr r4, =(0x080E4F9C +1)
	bx r4
	

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Increase Start Menu Height for Fewer Items
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixStartMenuSize:
	push {r2}
	mov r2, r1	@menu items
	lsl r0, r1, #0x19
	mov r1, #0xff
	lsl r1, r1, #0x18	@FF000000
	add r0, r0, r1
	lsr r0, r0, #0x18
	cmp r2, #0x5
	bhi ReturnStartMenuHeight
	add r0, #0x1
	
ReturnStartMenuHeight:
	pop {r2}
	str r0, [sp, #0x4]
	ldr r0, =(0x080f7900 +1)
	bx r0


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Reusable TMs
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
ReusableTMCheck1:
	mov r0, r1	@item ID
	push {r1}
	bl CheckReusableTMs
	pop {r1}
	cmp r0, #0x0
	bne IsReusable1
	mov r0, r1
	ldr r1, =(0x08124EAA +1)
	bx r1
IsReusable1:
	ldr r1, =(0x08124EB0 +1)
	bx r1
	
.align 2
.pool
ReusableTMCheck2:
	mov r0, r1	@item ID
	push {r1}
	bl CheckReusableTMs
	pop {r1}
	cmp r0, #0x0
	bne IsReusable2
	mov r0, r1
	ldr r1, =(0x08124F76 +1)
	bx r1
IsReusable2:
	ldr r1, =(0x08124F7C +1)
	bx r1

.align 2
.pool
ReusableTMCheck3:
	mov r0, r4	@item id
	bl CheckReusableTMs
	cmp r0, #0x0
	bne IsReusable3
	mov r0, r4
	ldr r1, =(0x08125C7E +1)
	bx r1
IsReusable3:
	ldr r1, =(0x08125C84 +1)
	bx r1
		
.align 2
.pool
SingleTmPurchaseFix:
	mov r0, r4	@item id
	mov r1, r6	@task id
	bl CheckTmPurchase
	pop {r4-r7, pc}
	
.align 2
.pool
AddSingleTmFix:
	bl CheckSingleBagTm
	cmp r0, #0xFF
	beq AddTmToBag
	strh r0, [r4, #0x2]
AddTmToBag:
	ldrh r0, [r4, #0xA]	@item
	ldrh r1, [r4, #0x2]	@qty
	bl BagAddItem
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	ldr r1, =(0x0809BEC6 +1)
	bx r1	
	
BagAddItem:
	ldr r2, =(0x809A084 +1)
	bx r2
	
.align 2
.pool
UnbuyableTmFix:
	mov r0, r4	@item id
	mov r1, r6	@task id
	bl CheckBuyableTm
	cmp r0, #0x0
	beq AskPurchaseQty
	pop {r4-r7, pc}
AskPurchaseQty:
	ldr r1, =(0x0809BC7C +1)
	bx r1
	
.align 2
.pool
UnsellableTmFix:
	ldrh r0, [r6]	@item id
	bl CheckSellTmHm
	cmp r0, #0x0
	beq Unsellable
	ldr r0, =(0x8132968 +1)
	bx r0
Unsellable:
	ldr r0, =(0x08132928 +1)
	bx r0
	

.align 2
.pool
FixBoughtTmPrice:
	mov r0, r1
	bl CheckTmPrice
	mov r1, r0
	ldr r0, =(0x0809B42C +1)
	bx r0

	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ TM/HM Expansion - Fix Mart Listings
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ 0809B4A4 via r0	
FixMartTmListing:
	mov r0, r7	@item id
	bl LoadTmHmNameInMart
	ldr r1, =(0x0809B4B2 +1)
	bx r1
	
.align 2
.pool
TmMartDescriptionFix:
	bl LoadTmHmMartDescription
	mov r7, r0
	ldr r0, =(0x0809B34A +1)
	bx r0
	

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ TM/HM Expansion - Fix Hm Symbol
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ 08131E96 via r1
HmSymbolFix:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10	@item id
	bl CheckHmSymbol
	mov r5, r0
	cmp r0, #0x0
	beq MakeHmSymbol
	cmp r0, #0x1
	beq ShowQty
SkipSymbolAndQty:
	ldr r0, =(0x08131EFE +1)
	bx r0
MakeHmSymbol:
	ldr r0, =(0x08131EF4 +1)
	bx r0
ShowQty:	
	ldr r0, =(0x08131EA6 +1)
	bx r0
	
.align 2
.pool
SelectedTmSymbolFix:
	ldrh r0, [r6]	@item ID
	bl CheckHmSymbol
	cmp r0, #0x0
	bne SkipHmSymbol
	ldr r0, =(0x08132516 +1)
	bx r0
SkipHmSymbol:
	ldr r0, =(0x08132528 +1)
	bx r0
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ TM/HM Expansion - Item Ordering
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
SortTmHms:
	push {r2-r6}
	add r1, r7, r0
	ldrh r0, [r1]	@ bagPocket->itemSlots[i].itemId
	bl CheckTmHmInFront
	pop {r2-r6}
	cmp r0, #0x1	@ true
	beq SkipInternal
	ldr r0, =(0x0809A63C +1)
	bx r0
SkipInternal:
	ldr r0, =(0x0809A66A +1)
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ TM/HM Expansion - Disc Image Loader
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ 081336BC via r0
.align 2
.pool
FixTmHmDiscLoader:
	mov r0, r4	@sprite* disc
	mov r1, r6	@ item id
	bl CheckDiscIsTmHm
	mov r5, r0	@tm Id
	ldr r0, =(0x081336C4 +1)
	bx r0
	
@ 08133854 via r0
.align 2
.pool
FixTmHmDiscLoader2:
	ldrh r1, [r5, #0x2e]	@ item id
	mov r0, r5
	bl CheckDiscIsTmHm
	strh r0, [r5, #0x2e]	@ tm id
	ldr r1, =(0x08133868 +1)
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ TM/HM Expansion - Disc Palette Loader
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ 08133748 via r1
FixTmHmDiscPal:
	push {r1-r3}
	mov r0, r4	@move type
	bl FixTmHmDiscPalette
	pop {r1-r3}
	cmp r0, #0x0
	beq DiscPalFromTable
	b ReturnDiscPal
	
DiscPalFromTable:
	ldr r0, =(0x8463238)
	lsl r4, r4, #0x1
	add r4, r4, r0
	ldrh r1, [r4]
	lsl r1, r1, #0x1
	ldr r0, [r2]
	add r0, r0, r1
	
ReturnDiscPal:
	ldr r1, =(0x08133754 +1)
	bx r1
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ TM/HM Expansion - Disc Position
@ 08133798
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
FixTmHmDiscPos:
	cmp r4, #0xFF
	beq EndOfCase
	mov r1, r4	@tm id
	bl TmHMDiscPosition
	ldr r4, =(0x081337B4 +1)
	bx r4	
	
EndOfCase:
	mov r6, #27
	mov r1, #54
	ldr r0, =(0x081337A0 +1)
	bx r0
	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Evolution Methods - Remove Evo Item
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
RemoveEvoItem:
	ldrh r0, [r4, #0x8]
	add r0, #0x1
	strh r0, [r4, #0x8]
	add r2, r2, r1
	add r2, #0xC
	push {r1-r5}
	mov r0, r9
	bl ItemEvolutionRemoval
	pop {r1-r5}
	mov r0, r9
	ldr r1, =(0x080CED68 +1)
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Unhidden Power - Write Type
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
WriteTypeHook:
	mov r0, r1	@move
	push {r0}
	ldrb r0, [r5]	@active battler
	lsl r0, #1
	ldr r1, =(0x02023BCE)
	add r0, r0, r1
	ldrb r0, [r0]
	mov r1, #100
	mul r0, r1
	ldr r1, =(0x02024284)
	add r1, r0, r1				@pokemon
	pop {r0}
	bl GetSummaryScreenMoveType
	ldr r1, =(0x0803098E +1)
	bx r1
	
	
.align 2
.pool
DisplayTypeHook:
	push {r3-r7}
	ldr r0, =(0x0203B140)	@pkmn_status_data
	ldr r0, [r0]
	ldr r1, .monLoc
	add r1, r0, r1		@pokemon	
	mov r0, r2	@move
	bl GetSummaryScreenMoveType
	pop {r3-r7}
	strh r0, [r4]
	ldr r0, [r6]
	ldr r2, =(0x081368D8 +1)
	bx r2
	
.align 2
.pool
.monLoc: .word 0x00003290


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Summary Screen Wrapping - hook at 13B20C via r1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
SummaryScreenWrapAround:
	push {r4-r7, lr}
	bl ChangeSummaryScreenMon
	pop {r4-r7, pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Whiteout Hack
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
DynamicWhiteoutMap:
	bl WhiteoutLogic
	cmp r0, #0x1
	beq NoHack
	ldr r0, =(0x080bfdf4|1)
	bx r0
NoHack:
	ldr r2, =(0x083eec98)
	sub r0, r6, #0x1
	lsl r0, r0, #0x2
	add r1, r0, r2
	ldrh r1, [r1]
	strb r1, [r4]
	add r2, #0x2
	add r0, r0, r2
	ldr r2, =(0x080bfd64|1)
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 10AD98 via r1
CheckDexNavSelect:
    bl CheckRegisteredSelect
    cmp r0, #0x0
    bne RestoreSelect
    ldr r1, =(0x810ADA0|1)
    bx r1

RestoreSelect:
   bl SelectHookRestore
   ldr r1, =(0x810ADA0|1)
   bx r1
   

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 6F3A8 via r3
PokeToolNoFade:
    cmp r1, r0
    beq EndStartOptFadeCheck
    ldr r0, =(PokeToolsFunc)
    cmp r1, r0
    beq EndStartOptFadeCheck
    ldr r0, =(0x080CCB68|1)
    bl CallViaR0
    ldr r0, =(0x0806F3B0|1)
CallViaR0:
    bx r0
EndStartOptFadeCheck:
    pop {pc}
	

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	08 47 at 5e5d4
@	ptr+1 at 5e5f4
.align 2
.pool
NpcSpawnWithTemplate:
	ldr r1, =(0x02036e38) @EventObjects
	add r5, r0, r1
	ldrb r0, [r5, #0x5] 
	ldrb r1, [r5, #0x1a]
	lsl r1, r1, #0x8
	orr r0, r1
	bl GetEventObjectGraphicsInfo
	ldr r1, =(0x0805e5dc+1)
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	hook at 5e510 via r1
.align 2
.pool
NpcSizeFix:	
	push {r4, lr}
	sub sp, #0x8
	mov r4, r0
	ldrb r0, [r4, #0x5] 
	ldrb r1, [r4, #0x1a]
	lsl r1, r1, #0x8
	orr r0, r1
	bl GetEventObjectGraphicsInfo
	ldr r1, =(0x0805e51c+1)
	bx r1
		
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	hook at 5e964 via r4
.align 2
.pool	
NewNpcInfo:
	mov r4, r0
	mov r5, r1
	mov r3, r2
	ldrb r0, [r4, #0x1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	ldrb r1, [r4, #0x3]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x10
	orr r0, r0, r1
	ldrb r1, [r4, #0x9]
	mov r2, r5
	ldr r4, =(0x0805e96e+1)
	bx r4
	 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	08 47 at 5ee84
@	ptr+1 at 5efe4
.align 2
.pool		 
LinkNpcFix:
	ldr r6, =(0x02036e38)
	add r6, r6, r0
	mov r0, #0x0
	str r0, [sp, #0x20]
	ldrb r0, [r6, #0x5] 
	ldrb r1, [r6, #0x1a]
	lsl r1, r1, #0x8
	orr r0, r1
	bl GetEventObjectGraphicsInfo
	mov r5, r0
	ldrh r2, [r5, #0x6]
	add r4, sp, #0x18
	ldr r0, [r4, #0x4]
	lsr r0, r0, #0x10
	lsl r0, r0, #0x10
	orr r0, r2
	str r0, [r4, #0x4]
	ldrb r0, [r6, #0x5] 
	ldrb r1, [r6, #0x1a]
	lsl r1, r1, #0x8
	orr r0, r1
	ldr r1, =(0x0805eea2+1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	hook at 67a12 via r5
.align 2
.pool
NpcOffscreenFix:
	add r5, r0, #0x0
	add r4, r1, #0x0
	ldrb r1, [r5,#1]
	mov r0, #0x41
	neg r0, r0
	and r0, r1
	strb r0, [r5,#1]
	ldrb r0, [r5, #0x5]
	ldrb r1, [r5, #0x1a]
	lsl r1, r1, #0x8
	orr r0, r1
	bl GetEventObjectGraphicsInfo
	ldr r1, =(0x08067a26+1)
	bx r1
	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	hook at 67f92 via r5
.align 2
.pool
NpcWaterFix:	
	mov r5, r0
	mov r4, #0x0
	ldrb r0, [r5, #0x5] 
	ldrb r1, [r5, #0x1a]
	lsl r1, r1, #0x8
	orr r0, r1
	bl GetEventObjectGraphicsInfo
	ldr r1, =(0x08067f9c+1)
	bx r1
	

	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ DYNAMIC OVERWORLD PALETTES

@@@@@@@@@@@@@@@@@@@@@@@@@@
@ hook at 9d5a8 via r2
.align 2
.pool
SetPalFossilImage: 
	push {r0}
	mov r0, #5
	ldr r1, =0x20370C0 @ var 8004
	ldrh r1, [r1]
	bl FindPalRef
	lsl r1, r0, #4
	pop {r0}
	ldrb r2, [r0,#5]
	mov r3, #0xF
	and r2, r3
	orr r1, r2
	ldr r2, =0x809D5B2+1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@
@ hook at 9D538, 9D568 via r1
.align 2
.pool
LoadPalFossilImage:
	push {r0}
	mov r0, #5
	ldr r1, =0x20370C0 @ var 8004
	ldrh r1, [r1]
	bl AddPalRef
	mov r1, r0
	pop {r0}
	cmp r1, #0xFF @ no more space
	beq LoadPalFossilImage_End
	lsl r1, #4
	ldr r2, =(0x80089A0+1) @ LoadOBJPalette
	bl CallByR2

LoadPalFossilImage_End:
	ldr r1, =0x809D572+1
	bx r1

CallByR2:
	bx r2	
	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ hook at 0x7280 via r1
.align 2
.pool
DeleteOBJFreeTiles:
	push {r4-r7,lr}
	mov r5, r0
	ldr r0, [r0,#0x14]
	ldr r1, =0x8231D00 @ template for empty OBJ
	cmp r0, r1
	beq DeleteOBJFreeTiles_Return
	ldrb r0, [r5,#5]
	lsr r0, #4
	bl PalRefDecreaseCount
DeleteOBJFreeTiles_Return:
	mov r0, r5
	add r0, #0x3E
	ldrb r1, [r0]
	ldr r0, =0x8007288+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x779C via r1
DeleteAllOBJs: 
	ldr r1, =0x80073DC+1 @ DeleteOBJ
	bl CallByR1
	bl ClearAllPalRefs
	pop {r4,r5,pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDAF88 via r0
DeleteReflection: 
	ldrb r0, [r4,#5]
	lsr r0, #4
	bl PalRefDecreaseCount
	mov r2, r4
	add r2, #0x3E
	ldrb r1, [r2]
	mov r0, #2
	neg r0, r0
	and r0, r1
	ldr r1, =0x80DAF94+1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDB120 via r0
DeleteWarpArrow: 
	push {lr}
	add r1, r2
	mov r2, r1
	add r2, #0x3E
	ldrb r0, [r2]
	mov r3, #5
	and r0, r3
	cmp r0, #1
	bne DeleteWarpArrow_SetBits @ only decrease count when arrow disappears
	ldrb r0, [r1,#5]
	lsr r0, #4
	push {r2}
	bl PalRefDecreaseCount
	pop {r2}
DeleteWarpArrow_SetBits:
	ldrb r0, [r2]
	mov r1, #4
	orr r0, r1
	strb r0, [r2]
	pop {pc}

CallByR1:
	bx r1
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool	
@ hook at 0x71B8 via r0
SetPalMisc: 
	mov r0, r8
	bl GetPalSlotMisc
	cmp r0, #0xFF
	beq SetPalMisc_DoNotSet
	ldr r1, =0x80071CA+1
	bx r1

SetPalMisc_DoNotSet:
	ldr r0, =0x80071D6+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool	
@ hook at 0x82664 via r3
SetPalEmotionBubble:
	push {r0-r2}
	ldr r0, .PalTagEmotionBubble
	bl FindOrLoadNPCPalette
	lsl r1, r0, #4
	pop {r0}
	ldrb r3, [r0,#5]
	mov r2, #0xF
	and r2, r3
	mov r3, #0xD
	neg r3, r3
	and r2, r3
	mov r3, #4
	orr r2, r3
	orr r2, r1
	strb r2, [r0,#5]
	pop {r1,r2}
	ldr r3, =0x8082670+1
	bx r3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDB1AC via r3
SetPalWarpArrow: 
	push {r2}
	ldr r0, .PalTagEmotionBubble
	bl FindOrLoadNPCPalette
	pop {r2}
	lsl r0, #4
	ldrb r3, [r2,#5]
	mov r1, #0xF
	and r1, r3
	orr r0, r1
	strb r0, [r2,#5]
	mov r3, r2
	add r3, #0x3E
	ldrb r1, [r3]
	mov r0, #5
	neg r0, r0
	and r0, r1
	ldr r1, =0x80DB1B6+1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x13F434 via r0
SetPalItemfinderArrow: 
	ldr r0, .PalTagEmotionBubble
	bl FindOrLoadNPCPalette
	lsl r0, #4
	ldrb r1, [r4,#5]
	mov r2, #0xF
	and r1, r2
	orr r0, r1
	strb r0, [r4,#5]
	mov r0, r4
	ldr r1, =0x813F43C+1
	bx r1

.align 2
.PalTagEmotionBubble: .word 0x1100

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x12D7A8 via r1
SetPalFameCheckerUnknown: 
	push {r0}
	ldr r0, .PalTagFameChecker
	push {r2}
	bl FindOrLoadNPCPalette
	pop {r2}
	lsl r0, #4
	ldrb r3, [r2,#5]
	mov r1, #0xD
	neg r1, r1
	and r1, r3
	mov r3, #8
	orr r1, r3
	mov r3, #0xF
	and r1, r3
	orr r1, r0
	pop {r0}
	ldr r3, =0x812D7BA+1
	bx r3

.align 2
.PalTagFameChecker: .word 0x1103

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool	
@ hook at 0x5E6B0 via r0, for regular NPCs
SetPalNPC1: 
	ldrh r0, [r6,#2]	@palette tag
	bl FindOrLoadNPCPalette
	lsl r1, r0, #4
	ldrb r2, [r4,#5]
	ldr r0, =0x805E6B8+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
 @ hook at 0x5EF78 via r0, for regular NPCs when returning to the map
SetPalNPC2:
	ldrh r0, [r5,#2]
	bl FindOrLoadNPCPalette
	lsl r1, r0, #4
	ldrb r2, [r4,#5]
	mov r0, #0xF
	and r0, r0, r2
	ldr r2, =0x805EF82+1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x5EAB0 via r0, for script command 0xAA createsprite
SetPalNPC3: 
	ldrh r0, [r4,#2]
	bl FindOrLoadNPCPalette
	lsl r1, r0, #4
	ldrb r3, [r5,#5]
	mov r2, #0xF
	ldr r0, =0x805EAB8+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x5EBC0 via r0, for Fame Checker NPCs
SetPalNPC4:
	mov r0, r4
	ldr r1, =0x80079FC+1 @ LoadAnimFrame, needed for instant refreshing
	bl CallByR1
	ldrh r0, [r7,#2]
	bl FindOrLoadNPCPalette
	lsl r1, r0, #4
	ldrb r3, [r4,#5]
	mov r2, #0xF
	ldr r0, =0x805EBC8+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDADB4 via r2
SetPalReflection: 
	lsr r0, r1, #4
	bl FindOrCreateReflectionPalette
	ldrb r1, [r7,#5]
	ldr r2, = 0x80DADBC+1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDC410 via r2
SetPalSurf: 
	push {r1}
	strb r0, [r3]
	ldr r0, .BlobPalette
	bl FindOrLoadNPCPalette
	pop {r1}
	lsl r0, #4
	ldrb r3, [r1,#5]
	mov r2, #0xF
	and r2, r3
	orr r0, r2
	ldr r2, =0x80DC418+1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x87188 via r1
SetPalFly:
	push {r0}
	ldr r0, .BlobPalette
	push {r2}
	bl FindOrLoadNPCPalette
	pop {r2}
	lsl r0, #4
	ldrb r3, [r2,#5]
	mov r1, #0xF
	and r1, r3
	mov r3, #0xD
	neg r3, r3
	and r1, r3
	orr r1, r0
	pop {r0}
	ldr r3, =0x8087192+1
	bx r3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x86D58 via r0
SetPalFly2: 
	push {r2}
	ldrh r0, .BlobPalette
	bl FindOrLoadNPCPalette
	pop {r2}
	lsl r0, #4
	ldrb r1, [r2,#5]
	mov r3, #0xF
	and r1, r3
	mov r1, #0xD
	neg r1, r1
	and r1, r3
	mov r3, #4
	orr r1, r3
	orr r0, r1
	ldr r3, =0x8086D68+1
	bx r3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0xDB260 via r0
SetPalShadow: 
	push {r2}
	ldr r0, .BlobPalette
	bl FindOrLoadNPCPalette
	pop {r2}
	lsl r0, #4
	ldrb r1, [r2,#5]
	mov r3, #0xF
	and r1, r3
	orr r0, r1
	strb r0, [r2,#5]
	mov r0, #0
	pop {r4,r5,pc}
	
.pool
.align 2
.BlobPalette: .word 0x00001100

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x12D2AC via r0, needed for instant refreshing
RefreshFameCheckerNPCs: 
	ldr r0, =0x0812c5ab
	ldr r1, [sp,#0x14]
	cmp r0, r1
	bne RefreshFameCheckerNPCs_DoNotColor
	push {r4}
	mov r0, #0x10
	lsl r0, #4
	mov r1, #0x60
	mov r2, #0x10
	mov r3, #0
	ldr r4, =0x8045274+1 @ FadePalette, color palettes black if fading in
	bl CallByR4
	pop {r4}

RefreshFameCheckerNPCs_DoNotColor:
	ldr r1, =0x8006BA8+1 @ RefreshOBJs
	bl CallByR1
	cmp r5, #1
	bne RefreshFameCheckerNPCs_Return2
	ldr r3, =0x203B0FC
	ldr r2, [r3]
	ldrb r0, [r2,#7]
	ldr r1, =0x812D2B4+1
	bx r1

RefreshFameCheckerNPCs_Return2:
	ldr r0, =0x812D2E0+1
	bx r0

CallByR4:
	bx r4	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool	
@ hook at 0x15D4E8 via r2
SetPalSSAnneWake: 
	push {r1}
	ldr r0, =0x1115
	bl FindOrLoadNPCPalette
	pop {r1}
	lsl r0, #4
	ldrb r2, [r1,#5]
	mov r3, #0xD
	neg r3, r3
	and r2, r3
	mov r3, #8
	orr r2, r3
	mov r3, #0xF
	and r2, r3
	orr r0, r2
	ldr r2, =0x815D4FC+1
	bx r2

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x15D600 via r2
SetPalSSAnneSmoke: 
	push {r0}
	ldr r0, =0x1115
	bl FindOrLoadNPCPalette
	lsl r1, r0, #4
	pop {r0}
	ldrb r2, [r0,#5]
	mov r3, #0xF
	and r2, r3
	orr r1, r2
	ldr r2, =0x815D60A+1
	bx r2	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A2D0 via r0
GetDarkeningType1: 
	mov r0, r4
	bl GetDarkeningTypeBySlot
	cmp r0, #0
	ldr r1, =0x807A2D8+1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A428 via r0
GetDarkeningType2: 
	mov r0, r4
	bl GetDarkeningTypeBySlot
	ldr r1, =0x807A430+1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A544 via r0
GetDarkeningType3: 
	mov r0, r4
	bl GetDarkeningTypeBySlot
	ldr r1, =0x807A54C+1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
 @ hook at 0x7AEC4 via r1, dynamically brighten or un-brighten palettes when fog emerges or vanishes
FogBrightenAll:
	mov r1, #3
	and r1, r3
	cmp r1, #0
	bne FogBrightenAll_End
	lsr r2, #24
	cmp r2, #0
	bne FogBrightenAll_End
	bl FogBrightenPalettes
	
FogBrightenAll_End:
	ldr r6, .hword732
	add r0, r5, r6
	ldrh r1, [r0]
	lsl r1, r1, #8
	ldr r0, =0x807AECC | 1
	bx r0

.align 2
.hword732: .word 0x732

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7AAAC via r0
FogBrightenOnStep:
	add r2, #10
	add r0, r1, r2
	ldrb r0, [r0]
	push {r1,r2}
	bl GetFadeTypeByWeather
	pop {r1,r2}
	cmp r0, #2
	beq FogBrightenOnStep_Brighten @ only brighten if there is fog weather
	ldr r0, =0x807AAB6+1
	bx r0

FogBrightenOnStep_Brighten:
	mov r0, r4
	mov r1, #0
	mov r2, #0
	bl FogBrightenAndFade
	ldr r0, =0x807AB68+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A644 via r0
FogBrightenAndFadeIn: 
	mov r0, r4
	mov r1, r7
	ldrh r2, [sp]
	bl FogBrightenAndFade
	add r0, r4, #1
	mov r9, r0
	ldr r0, =0x807A6F8+1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A0A8 via r1, call the correct fade-in function for all fog types
GetFadeType1:
	push {r2}
	ldrb r0, [r0]
	bl GetFadeTypeByWeather
	pop {r2}
	lsl r0, #2
	ldr r1, =GetFadeType1_ReturnTable
	add r0, r1
	ldr r0, [r0]
	bx r0

.align 2, 0
GetFadeType1_ReturnTable:
	.word 0x807A16C+1
	.word 0x807A0F8+1
	.word 0x807A144+1
	.word 0x807A120+1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
@ hook at 0x7A860 via r2, copy palettes before fading out for all fog types
GetFadeType2: 
	push {r1}
	ldrb r0, [r0]
	bl GetFadeTypeByWeather
	pop {r1}
	cmp r0, #0
	beq GetFadeType2_NormalFade
	ldr r0, =0x807A872+1
	bx r0

GetFadeType2_NormalFade:
	ldr r0, =0x807A880+1
	bx r0
	
	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ SPECIALS
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Activate Wild Ground Battle
	ptr+1 at 15ffbc
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
.align 2
.pool
sp097_WildGroundBattle:
	push {r4-r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x8
	ldr r0, =(0x01000202)	@grass block data
	lsl r4, r0, #0x18
	lsr r4, r4, #0x18
	ldr r1, =(0x0806cbe4|1)
	bx r1
	
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Activate Wild Sea Battle
	ptr+1 at 15ffc0
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
.align 2
.pool
sp098_WildSeaBattle:
	push {r4-r7, lr}
	mov r7, r8
	push {r7}
	sub SP, SP, #0x8
	ldr r0, =(0x22000410)	@water tile data
	lsl r4, r0, #0x18
	lsr r4, r4, #0x18
	ldr r1, =(0x0806cbe4|1)
	bx r1

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Createsprite from any Table
	repoint cmd at 15fc5c
	usage:
		writebytetooffset 0x[table] <gCreateSpriteTableSetter>
		createSprite 0x[OW] etc etc
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
.align 2
.pool
CreateSpriteLoadTable:
	push {r4-r6,lr}
	mov r6, r8
	push {r6}
	sub sp, #0x8
	mov r5, r0
	ldr r0, [r5, #0x8]
	ldrb r1, [r0]	@sprite number
	ldr r6, =gCreateSpriteTableSetter
	ldrb r6, [r6]
	cmp r6, #0x0
	beq createSprite
getTable:
	lsl r6, r6, #0x18
	lsr r6, r6, #0x10
	orr r1, r6	
createSprite:
	mov r8, r1
	ldr r6, =(0x0806b660 +1)
	bx r6

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Ghost Battle Special 0x156
	hook at 0807f92a via r2
	Inputs:
		var8004: species
		var8005: level
		var8006: hold item
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
.align 2
.pool
sp156_GhostBattleSpecial:
	mov r1, #0xfe
	str r1, [sp]
	mov r1, #0xc
	str r1, [sp, #0x4]
	mov r1, #0x0
	str r1, [sp, #0x8]
	ldr r3, =var8004
	ldrh r1, [r3]		@species
	cmp r1, #0x0
	beq defaultGhost
	ldrh r2, [r3, #0x2]	@level
	cmp r2, #MAX_LEVEL
	bgt defaultGhost
	cmp r2, #0x0
	beq defaultGhost
	ldrh r3, [r3, #0x4]	@hold item
	ldr r4, =(0x0807f93c|1)
	bx r4
defaultGhost:
	ldr r1, =SPECIES_MAROWAK
	ldr r2, =(0x0807f938|1)
	bx r2


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ hook at 46116 via r0
.align 2
.pool
DaycareInheritHiddenAbility:
	mov r4, sp
	add r0, r5, #0x0
	ldr r3 , func_roll_gender
	bl bx_r3
	cmp r0, #0xFE
	beq mother_in_slot_one
	mov r0, r5
	add r0, r0, #0x8C
	mov r1, #0xB
	bl get_zero_attr
	cmp r0, #0x84
	beq mother_in_slot_one
	mov r0, r5
	add r0, r0, #0x8C
	b check_for_hidden

mother_in_slot_one:
	add r0, r5, #0x0

check_for_hidden:
	mov r1, #0x2E
	bl get_zero_attr
	cmp r0, #0x0
	beq end
	ldr r3, func_rand
	bl bx_r3
	lsr r0, r0, #0x8
	cmp r0, #0x99
	bhi end
	ldr r3, func_set_attr
	mov r2, #0x1
	push {r2}
	mov r2, sp
	mov r1, #0x2E
	mov r0, r4
	bl bx_r3
	add sp, #0x4

end:
	mov r2, r4
	add r2, r2, #0x6A
	mov r0, #0x1
	strb r0, [r2]
	mov r0, r4
	ldr r1, func_return
	bx r1

get_zero_attr:
	mov r2, #0x0
	ldr r3, func_get_attr

bx_r3:
	bx r3

.align 2
func_roll_gender: .word 0x0803F730 + 1
func_get_attr: .word 0x0803FBE8 + 1
func_rand: .word 0x08044EC8 + 1
func_set_attr: .word 0x0804037C + 1
func_return: .word 0x08046120 + 1




@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@Swap Yes/No Outcomes
@	hook at 8ed8c via r1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
pcSelect_SwapDecision:
	push {r4}
	lsl r0, r0, #0x18
	asr r1, r0, #0x18
	ldr r4, =pcSelect_StateTracker
	ldrb r4, [r4]
	cmp r4, #0x3
	bne RegularChecks
SwapChecks:
	pop {r4}
	cmp r1, #0x0	@'yes'
	beq StoreAndCancel
	cmp r1, #0x0
	bgt CheckSwapNo
	mov r0, #0x1
	neg r0, r0
	cmp r1, r0
	beq ResetSwapandContinue
CheckSwapNo:
	cmp r1, #0x1	@'no'
	beq ResetSwapandContinue
DefaultCase:
	ldr r0, =(0x0808ee04 +1)	@do nothing
	bx r0
	
StoreAndCancel:
	ldr r0, =BoxDMA
	ldr r0, [r0]
	ldrb r0, [r0]	@ current box
	ldr r1, =var8000
	strh r0, [r1]	@ store current box to var8000
	ldr r0, =(0x02039821)	@box pos
	ldrb r0, [r0]
	strh r0, [r1, #0x2]		@store box pos to var8001
	bl StoreSpeciesID
	ldr r0, =pcSelect_StateTracker
	mov r1, #0xFE
	strb r1, [r0]	@ force exit from multichoice
	ldr r1, =var800d
	mov r0, #0x0
	strh r0, [r1]	@ set lastresult to 0
CancelOperations:
	ldr r0, =(0x0808edb8 +1)
	bx r0
	
ResetSwapandContinue:
	ldr r0, =pcSelect_StateTracker
	mov r1, #0x2
	strb r1, [r0]	@ undo yes/no swap
	b ContinueOperations

RegularChecks:
	pop {r4}
	cmp r1, #0x0	@yes
	beq ContinueOperations
	cmp r1, #0x0
	bgt CheckNo
	mov r0, #0x1
	neg r0, r0
	cmp r1, r0
	beq CancelRegular
	b DefaultCase
	
CheckNo:
	cmp r1, #0x1	@'no'
	beq CancelRegular
	b DefaultCase
	
ContinueOperations:
	ldr r0, =(0x0808eda8 +1)	@ remove yes/no box
	bx r0
	
CancelRegular:	
	ldr r1, =pcSelect_StateTracker
	ldrb r0, [r1]
	cmp r0, #0x0
	beq CancelOperations	@ if not in hack, just cancel
@ else, set relevant ram
	mov r0, #0x7F
	ldr r1, =var800d	@ set lastresult 0x7f if cancelling out
	strh r0, [r1]
	ldr r1, =pcSelect_StateTracker
	mov r0, #0xFE
	strb r0, [r1]
	b CancelOperations	

StoreSpeciesID:
	push {r2,lr}
	ldr r1, =(30*80)	@ size per box
	ldr r0, =var8000
	ldrh r0, [r0]	@box num
	mul r0, r1
	ldr r1, =BoxDMA
	ldr r1, [r1]
	add r1, #0x4	@start of box data	
	add r0, r0, r1
	ldr r1, =var8001
	ldrh r1, [r1]	@ box entry num
	mov r2, #80		@ size per entry
	mul r1, r2
	add r0, r0, r1	@ data of selected poke
	mov r1, #0xb
	bl GetAttr
	ldr r1, =var8001
	strh r0, [r1, #0x2]	@store species ID to var8002
	pop {r2,pc}
	
GetAttr:
	ldr r2, =(0x0803fbe8 +1)
	bx r2


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@Discern between closing box and Selection
@	hook at 8ebb2 via r0
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
pcSelect_ClosingCheck:
	ldr r1, =pcSelect_StateTracker
	ldrb r0, [r1]
	cmp r0, #0x0
	beq ProceedCloseBox
IndicateCloseBox:
	mov r0, #0xFE	@ force exit from multichoice
	strb r0, [r1]
	mov r0, #0x7F
	ldr r1, =var800d
	strh r0, [r1]	@ store 0x7f into lastresult if closing without selecting
ProceedCloseBox:
	ldr r0, =(0x020397b0)
	ldr r0, [r0]
	ldrb r0, [r0]
	cmp r0, #0x4
	bls LoadTable
	pop {r4}
	pop {r0}
	bx r0
LoadTable:
	ldr r1, =(0x0808ebbe +1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@Hack Withdraw Box Options
@	hook via r0 at 94982
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
pcSelect_HackWithdrawBRM:
	cmp r1, #0x0
	beq NoPokeSelected
CheckHack:
	ldr r0, =pcSelect_StateTracker
	ldrb r0, [r0]
	cmp r0, #0x0
	beq WithdrawBRM
CustomBRM:
	mov r0, #0x26	@select
	bl BRMAdd
	mov r0, #0x6	@summary
	bl BRMAdd
	ldr r0, =(0x080949fe +1)
	bx r0	
	
WithdrawBRM:
	mov r0, #0x2	@withdraw
	bl BRMAdd
	ldr r0, =(0x080949c0 +1)
	bx r0
	
NoPokeSelected:
	ldr r0, =(0x080949bc +1)
	bx r0

BRMAdd:
	ldr r1, =(0x08094dec +1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@Change 'Party' String
@	hook via r0 at 8d3b8
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
pcSelect_NoPartyString:
	ldr r0, =pcSelect_StateTracker
	ldrb r0, [r0]
	cmp r0, #0x0
	beq RegularPartyMsg
BetterPartyMsg:
	mov r0, #0x1F
	b LoadPartyMsg
RegularPartyMsg:
	mov r0, #0x10
LoadPartyMsg:
	bl LoadFromTable
	ldr r1, [r4]
	ldr r0, =(0x0808d3c0 +1)
	bx r0
	
LoadFromTable:
	ldr r1, =(0x0808fba4 +1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Replace Box Cancel Msg
@	hook at 8ed54 via r0
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
pcSelect_OperationsMsg:
	mov r0, #0x5
	bl play_audio
	ldr r0, =pcSelect_StateTracker
	ldrb r0, [r0]
	cmp r0, #0x3	@ selected poke -> ask to select
	beq AskTradeOp
	mov r0, #0x12	@'continue box operations?' -> else, hit 'B' in menu as normal
	b LoadOperation
AskTradeOp:
	mov r0, #0x20	@ask select
LoadOperation:
	ldr r1, =(0x0808ed5c +1)
	bx r1

play_audio:
	ldr r1, =(0x080722cc +1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Skip Multichoice box Generation
@	hook at 8c3e0 via r0
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
pcSelect_SkipMultiGen:
	mov r0, #0x1c
	bl help_system_set_context
	ldrb r0, [r5, #0xa]
	mov r1, r5
	add r1, #0x26
	ldr r2, =pcSelect_StateTracker
	ldrb r2, [r2]
	cmp r2, #0x0
	beq CreateMultiBoxes
SkipMultiBoxes:
	ldr r0, =(0x0808c3fc +1)
	bx r0	
	
CreateMultiBoxes:
	ldr r2, =(0x0808c3ec +1)
	bx r2
	
help_system_set_context:
	ldr r1, =(0x0812b1f0 +1)
	bx r1


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Remove Pokemon Limit from Withdraw
@	hook at 8c53c via r1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
pcSelect_SkipWithdrawCount:
	ldr r1, =pcSelect_StateTracker
	ldrb r1, [r1]
	cmp r1, #0x0
	beq CheckWithdrawLimit
	mov r0, #0xFF	@ remove limit
CheckWithdrawLimit:
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bne SpaceAvailable
	ldr r0, =(0x0808c544 +1)
	bx r0
SpaceAvailable:
	ldr r1, =(0x0808c564 +1)
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Force Multichoice Selections
@	hook at 10f99c via r1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
pcSelect_ForceMulti:
	ldr r1, =pcSelect_StateTracker
	ldrb r1, [r1]
	cmp r1, #0x0
	beq RegularButtonChecks
	cmp r1, #0x1		@ activate withdraw
	beq ForceWithdraw
	cmp r1, #0xFE
	bne RegularButtonChecks
ForceExit:
	ldr r0, =(0x0810f9cc +1)
	bx r0
ForceWithdraw:
	ldr r0, =pcSelect_StateTracker
	mov r1, #0x2
	strb r1, [r0]
	ldr r0, .choice_curr
	mov r4, #0x0
	strb r4, [r0]	@force first choice (withdraw)
	ldr r4, =(0x0203ade4)	@choice_x
	ldr r0, =(0x0810f9b4 +1)
	bx r0	
	
RegularButtonChecks:	
	ldrh r1, [r0, #0x2e]	@button pressed
	mov r0, #0x1	@'a'
	and r0, r1
	cmp r0, #0x0
	beq CheckB
PressA:
	ldr r4, =(0x0810f9a6 +1)
	bx r4
CheckB:
	ldr r0, =(0x0810f9c4 +1)
	bx r0
	
.align 2
.choice_curr: .word 0x0203ade6


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@Choose 'Select'
@	hook at 8d934 via r1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
pcSelect_ChooseSelect:
	bl CheckButtons	@ check buttons and return index
	add r0, #0x1
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	cmp r0, #0x27	@ select index+1
	bge DoSelectFunc
	ldr r1, =(0x0808d93e +1)
	bx r1
	
DoSelectFunc:	
	ldr r0, =pcSelectionFunc
	mov pc, r0
	
CheckButtons:
	ldr r1, =(0x08094f94 +1)
	bx r1
	
.align 2
pcSelectionFunc:
	bl BoxClosing
	ldr r0, =pcSelect_StateTracker
	mov r1, #0x3
	strb r1, [r0]
	ldr r0, =(0x0808ece4 +1)	@ cancel box func
	bl dp17_task_set_function
	pop {r4}
	pop {r0}
	bx r0

BoxClosing:
	ldr r0, =(0x0808fd20 +1)
	bx r0
	
dp17_task_set_function:
	ldr r1, =(0x0808cff8 +1)
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Physical Special Split Icon Loading Routine
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.equ SUMMARY_SCREEN_DATA, 0x203B140

.align 2
.pool
PhysicalSpecialSplitIconRoutine:
	push {r0-r4}
	ldr r0, =0x203B16D
	ldrb r1, [r0]
	lsl r1, #0x1
	ldr r0, =SUMMARY_SCREEN_DATA
	ldr r0, [r0]
	ldr r3, .3258
	add r0, r3
	add r0, r1
	ldrh r3, [r0]
	cmp r3, #0x0
	beq PSSIconRoutineEnd
	lsl r0, r3, #0x1
	add r0, r3
	lsl r0, #0x2
	ldr r1, =gBattleMoves
	add r0, r1
	ldrb r4, [r0, #0xA]
	lsl r0, r4, #0x1
	add r4, r0
	lsl r4, #0x6
	ldr r0, =PSSIconsTiles
	add r0, r4
	ldr r1, =0x6001800
	mov r2, #0x30
	swi 0xB
	ldr r0, =PSSIconsTiles
	add r0, r4
	add r0, #0x60
	ldr r1, =0x6001800
	mov r2, #0x1E
	lsl r2, #0x4
	add r1, r2
	mov r2, #0x30
	swi 0xB

PSSIconRoutineEnd:
	pop {r0-r4}
	ldr r4, =0x203B148
	ldr r0, [r4]
	ldr r1, =0x813A17C | 1
	bx r1

.align 2
.3258: .word 0x3258

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Expanded PC BRM Table
@	pointer 94e48
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
pcSelect_newBrmTable:
.word 0x08418468
.word 0x0841846F
.word 0x08418475
.word 0x08418484
.word 0x0841847E
.word 0x08418489
.word 0x0841848F
.word 0x08418497
.word 0x0841849F
.word 0x084184A9
.word 0x084184AE
.word 0x084184A4
.word 0x084184B8
.word 0x084184BD
.word 0x084184BD
.word 0x084184C2
.word 0x084184C9
.word 0x084184CD
.word 0x084184D2
.word 0x084184DC
.word 0x084184E6
.word 0x084184F0
.word 0x084184F9
.word 0x08418500
.word 0x08418505
.word 0x0841850C
.word 0x08418514
.word 0x08418519
.word 0x08418521
.word 0x08418526
.word 0x0841852B
.word 0x08418531
.word 0x0841853A
.word 0x08418540
.word 0x08418544
.word 0x0841854A
.word 0x08418555
.word 0x0841855B
.word select_str	@0x26

select_str: .byte S_, e_, l_, e_, c_, t_, 0xff

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ New Operations String Table
@	pointers 8fbc8, 8fce4 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align 2
.pool
pcSelect_OperationsTable:
.word 0x08418208,0x0
.word 0x0841821b,0x0
.word 0x08418233,0x0
.word 0x08418248,0x0
.word 0x0841825c,0x1
.word 0x0841826c,0x0
.word 0x0841827f,0x0
.word 0x08418295,0x1
.word 0x084182a7,0x0
.word 0x084182b8,0x0
.word 0x084182ce,0x4
.word 0x084182df,0x6
.word 0x084182ec,0x0
.word 0x084182FF,0x0
.word 0x08418319,0x0
.word 0x0841832c,0x0
.word 0x08418346,0x0
.word 0x0841835f,0x0
.word 0x08418379,0x0
.word 0x08418392,0x1
.word 0x084183a0,0x0
.word 0x084183ba,0x0
.word 0x084183c5,0x0
.word 0x0841825c,0x7
.word 0x084183dd,0x0
.word 0x084183f0,0x7
.word 0x08418408,0x0
.word 0x08418419,0x0
.word 0x08418433,0x7
.word 0x08418443,0x7
.word 0x08418452,0x0
.word selectPoke_str, 0x0	@ 0x1F
.word askSelect_str, 0x1		@ 0x20

askSelect_str: .byte S_, e_, l_, e_, c_, t_, Space, 0xf7, 0x0, Interro, 0xff
selectPoke_str: .byte S_, e_, l_, e_, c_, t_, Space, a_, Space, P_, o_, k_, Poke_e, m_, o_, n_, Dot, 0xff

