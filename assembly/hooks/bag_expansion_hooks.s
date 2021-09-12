.thumb
.text
.align 2

.include "../asm_defines.s"

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetCurrentPocketItemAmountHook:
	push {r1-r3, lr}
	bl GetCurrentPocketItemAmount
	pop {r1-r3, pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
CountItemsInBag:
	push {r4-r6,lr}
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	bl StoreBagItemCount
	lsl r1, r5, #0x3
	ldr r0, =0x8108DD0 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
GetSmallestItemAmount:
	push {r1-r2}
	mov r0, r5
	bl GetNumItemsInPocket
	pop {r1-r2}
	ldr r3, =0x8108E29
	bx r3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*.pool
OpenBagBugFix1:
	push {r2-r3}
	mov r0, r4
	bl GetNumItemsInPocket
	mov r1, r0
	pop {r2-r3}
	add r0, #0x1
	cmp r2, r0
	ldr r0, =0x8108A26 | 1
	bx r0
*/
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*.pool
OpenBagBugFix2:
	push {r1-r2}
	mov r0, r4
	bl GetNumItemsInPocket
	pop {r1-r2}
	add r3, r0, #0x1
	ldr r4, =0x8108A4C | 1
	bx r4
*/
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
OpenBagBugFix3:
	push {r2-r3}
	mov r0, r6
	bl GetNumItemsInPocket
	add r0, #0x1
	pop {r2-r3}
	ldr r1, =0x8108AC8 | 1
bxr1:
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x810852E with r0
BagLRScroll:
	ldrb r0, [r3, #0x16]
	mov r1, #0x1 @LIST_MULTIPLE_SCROLL_DPAD
	lsl r1, #0x6
	bic r0, r1 @Remove possible old settings
	mov r1, #0x2 @LIST_MULTIPLE_SCROLL_L_R
	lsl r1, #0x6
	orr r0, r1
	strb r0, [r3, #0x16]
	bl FixCubeCursorDefaultColour
	pop {r3}
	mov r8, r3
	pop {r4-r7, pc}

@0x8131D1C with r0
TMCaseLRScroll:
	ldrb r0, [r2, #0x16]
	mov r1, #0x1 @LIST_MULTIPLE_SCROLL_DPAD
	lsl r1, #0x6
	bic r0, r1 @Remove possible old settings
	mov r1, #0x2 @LIST_MULTIPLE_SCROLL_L_R
	lsl r1, #0x6
	orr r0, r1
	strb r0, [r2, #0x16]
	pop {r3}
	mov r8, r3
	pop {r4-r7, pc}

@0x813D306 with r0
BerryPouchLRScroll:
	ldrb r0, [r5, #0x16]
	mov r1, #0x1 @LIST_MULTIPLE_SCROLL_DPAD
	lsl r1, #0x6
	bic r0, r1 @Remove possible old settings
	mov r1, #0x2 @LIST_MULTIPLE_SCROLL_L_R
	lsl r1, #0x6
	orr r0, r1
	strb r0, [r5, #0x16]
	pop {r3}
	mov r8, r3
	pop {r4-r7, pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
AllocateBerryPouchSpace:
	push {r4,lr}
	bl AllocateBerryPouchListBuffers
	ldr r1, =0x813D1CE | 1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x806BDEC with r0
ExpandedBerriesFix1:
	cmp r6, #0x1
	bls BerryFix1NotBerry
	mov r0, r5
	bl IsBerry
	cmp r0, #0x0
	beq BerryFix1NotBerry
	ldr r0, =0x806BDFC | 1
	bx r0

BerryFix1NotBerry:
	ldr r0, =0x806BE22 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x814A490 with r0
ExpandedBerriesFix2: @Get Berry Number
	cmp r6, #0x0
	beq BerryFix2Return
	mov r0, r4
	bl BerryIdFromItemId

BerryFix2Return:
	ldr r0, =0x814A4C2 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x813D33C with r0
GetBerryNameBerryIdFix:
	mov r0, r5
	bl BerryIdFromItemId
	mov r1, r0
	mov r0, r6
	mov r2, #0x2
	ldr r3, =0x813D344 | 1
	bx r3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
PokeDudeBackupTMCaseHook:
	bl PokeDudeBackupKeyItemsTMs
	ldr r0, =0x8132E98 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
PokeDudeClearTMCaseHook:
	ldr r0, =0x8132EC4 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
PokeDudeRestoreTMCaseHook:
	bl PokeDudeRestoreKeyItemsTMs
	ldr r0, =0x81331A4 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.pool
@0x80143D4 wirh r0
AreItemsDisabledHook:
	bl IsBagDisabled
	cmp r0, #0x0
	beq CanUseItemsReturn
	ldr r0, =0x80143E0 | 1
	bx r0

CanUseItemsReturn:
	ldr r0, =0x8014428 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.pool
@0x810900C with r1
BagSorterHook:
	add r0, #0x1
	cmp r4, r0
	bne BagMenu_PressedAButton
	mov r0, r6
	bl TrySetupSortBag
	ldr r0, =0x81090C6 | 1
	bx r0

BagMenu_PressedAButton:
	ldr r0, =0x8109044 | 1
	bx r0

.pool
@0x8109AD4 with r0
BagSorterHook2:
	ldr r0, =var800e
	ldrh r0, [r0]
	ldr r1, .BagSortIndicator
	cmp r0, r1
	bne NotBagSortOption
	bl LoadBagSorterMenuOptions

NotBagSortOption:
	ldrb r1, [r7]
	sub r1, #0x1
	lsl r1, #0x18
	lsr r1, #0x18
	ldr r0, =0x8109ADC | 1
	bx r0

.pool
@0x8109B5C with r1
BagSorterHook3:
	mov r4, r0
	lsl r4, #0x18
	lsr r4, #0x18
	ldr r0, =Var800E
	ldrh r0, [r0]
	ldr r1, .BagSortIndicator
	cmp r0, r1
	bne PrintRegularItemSelected
	mov r0, r4
	bl PrintBagSortItemQuestion
	ldr r0, =0x8109B90 | 1
	bx r0

PrintRegularItemSelected:
	ldr r0, =Var800E
	ldr r1, =0x8109B64 | 1
	bx r1

.align 2
.BagSortIndicator: .word 0xF9F9

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.pool
@0x810948A with r0
PrepRedrawItemDescriptionOnEndMovingItem:
	mov r9, r4
	bl ForceRedrawItemDescription
	pop {r4-r6, pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.pool
@0x810872C with r0
PrintRegisteredItemInBagHook:
	mov r0, r7
	bl IsItemRegistered
	cmp r0, #0x0
	bne PrintRegisteredItem
	ldr r0, =0x8108752 | 1
	bx r0

PrintRegisteredItem:
	ldr r0, =0x810873E | 1
	bx r0

.pool
@0x8109A44 with r0
RegisteredItemBagActionsHook:
	mov r7, r3
	ldr r0, =Var800E
	ldrh r0, [r0]
	push {r2}
	bl IsItemRegistered
	pop {r2}
	cmp r0, #0x0
	bne RegisteredItemBagAction
	ldr r0, =0x8109A74 | 1
	bx r0

RegisteredItemBagAction:
	ldr r0, =0x8109A58 | 1
	bx r0

.pool
@0x810A02A with r0
RegisterItemHook:
	mov r0, r2
	bl HandleItemRegistration
	ldr r0, =0x810A052 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x81334C4 with r0
TMCaseSplitIconsHook:
	mov r0, r7 @Move (type is already in r1)
	bl PrintTMCaseTypeAndSplitIcons
	ldr r0, =0x81334CE | 1
	bx r0
