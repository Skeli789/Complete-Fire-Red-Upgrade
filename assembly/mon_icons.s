.thumb
.align 2

.global PrintMonIcons_hook
PrintMonIcons_hook:
add r0, r0, r4
lsl r0, r0, #3
add r1, r0, r1
mov r2, #8

    push {r0-r7}
    
    bl PrintMonIcons
    
    pop {r0-r7}
    ldr r0, =0x0800C6D4|1
    bx r0

.pool 
