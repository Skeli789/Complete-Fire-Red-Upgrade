.text
.align 2
.thumb
.thumb_func
.global box_func
.global box_func_end

.align 2
.pool
box_func:
    push {r4, lr}
    ldr r4, =0x2037101 @free RAM space
    LDR R0, =0x0202402C @free table RAM space
    LDR R1, =0x020370B8 @var8000
    mov r2, #0
    strb r2, [r0]
    LDRH R2, [R1]
    STRB R2, [R0, #0x1] @X
    LDRH R2, [R1, #0x2]
    STRB R2, [R0, #0x2] @Y
    LDRH R2, [R1, #0x4]
    STRB R2, [R0, #0x3] @W
    LDRH R2, [R1, #0x6]
    STRB R2, [R0, #0x4] @H
    mov r2, #0xf
    STRB R2, [R0, #0x5]
    mov r2, #0x8
    STRB R2, [R0, #0x6]
    mov r2, #0x0
    STRB R2, [R0, #0x7]
    mov r2, #76
    STRB R2, [R0, #0x8]
    mov r2, #0x9f
    STRB R2, [R0, #0x9]
    mov r2, #0x41
    STRB R2, [R0, #0xa]
    mov r2, #0x8
    STRB R2, [R0, #0xb]
    ldr r3, =0x8003ce5
    bl call_r3
    strb r0, [r4]
    ldr r3, =0x8003fa1
    bl call_r3
    ldrb r0, [r4]
    mov r1, #0
    ldr r3, =0x80F6F1D @Something with BGs
    bl call_r3
    bl print_string
    ldr r1, =box_func
    ldr r0, =print_string
    add r0, r0, r1
    mov r1, #2
    pop {r4, pc}

print_string:
    push {r4-r7, lr}
    sub sp, sp, #0xC
    add r7, r0, #0
    ldr r5, =0x2021D18 @displayed_string
    ldr r1, =0x3000F14 @loader 0x0
    ldr r1, [r1]
    mov r0, r5
    ldr r3, =0x8008FCD @body_string_loader
    bl call_r3
    ldr r0, =0x2037101 @free RAM space
    ldrb r0, [r0]
    mov r1, #3
    str r1, [sp]
    mov r1, #0xff
    str r1, [sp, #4]
    mov r1, #0
    str r1, [sp, #8]
    mov r1, #2
    add r2, r5, #0
    mov r3, #4
    ldr r4, =0x8002C49 @writeTextToTilemap
    bl call_r4
    ldr r0, =0x2037101 @free RAM space
    ldrb r0, [r0]
    mov r1, #2
    ldr r3, =0x8003F21 @writeBoxesToTilemap
    bl call_r3
    add sp, sp, #0xC
    pop {r4-r7, pc}

    call_r3:
    bx r3

    call_r4:
    bx r4

    .align 2

.pool
box_func_end:
    push {r0-r3, lr}
    ldr r0, =0x02001840
    ldr r3, =0x0600F800
    mov r1, #0xFF
    mov r2, #0

loop:
    str r2, [r0]
    str r2, [r3]
    add r0, #4
    sub r1, r1, #1
    bpl loop
    pop {r0-r3, pc}
