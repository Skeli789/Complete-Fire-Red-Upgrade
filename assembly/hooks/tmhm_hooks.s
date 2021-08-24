.text
.align 2
.thumb
.thumb_func

.include "../asm_defines.s"

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
.global FixTmShopPrice


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Reusable TMs
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
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

.pool
SingleTmPurchaseFix:
	mov r0, r4	@item id
	mov r1, r6	@task id
	bl CheckTmPurchase
	pop {r4-r7, pc}

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

.pool
FixTmShopPrice:
    mov r0, r6  @window
    mov r2, r5  @y
    bl PrintTmPriceOrPurchased
    add sp, sp, #0x14
    pop {r4-r6, pc}

.pool
ReloadMartList:
    mov r0, r5  @taskID
    bl ReloadMartListForTmPurchase
    ldr r0, =(0x0809BED4 +1)
    bx r0

.pool
@0x8125FC8 with r2
RemoveTMPPRestoreHook:
	mov r0, r6 @Mon
	bl NewTMReplaceMove
	ldr r0, =0x8125FD0 | 1
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


@ TM/HM Expansion - Fix Hm Symbol
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


@ TM/HM Expansion - Item Ordering
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


@ TM/HM Expansion - Disc Image Loader
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


@ TM/HM Expansion - Disc Palette Loader
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
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ 08133798
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

