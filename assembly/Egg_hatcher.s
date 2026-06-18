.text
.align 2
.thumb
.thumb_func

.global Egg_hatcher_asm
Egg_hatcher_asm:
    push {r4-r5, lr}
    ldr r4, =(0x20370C0)
    ldrh r4, [r4]
    cmp r4, #0x7
    beq Cancel
    ldr r5, =(0x80BFC14 +1)
    bl Linker
    ldr r5, =(0x20370D0)
    ldrh r5, [r5]
    cmp r5, #0x0
    beq MonIsNotEgg
    cmp r5, #0x1
    beq MonIsEgg

MonIsEgg:
    ldr r5, =(0x8046FD4 +1)
    bl Linker
    pop {r4-r5, pc}

MonIsNotEgg:
    mov r4, #0x0
    strh r4, [r5]
    pop {r4-r5, pc}
  
Cancel:
    ldr r5, =(0x20370D0)
    mov r4, #0x7
    strh r4, [r5]
    pop {r4-r5, pc}

Linker:
    bx r5
