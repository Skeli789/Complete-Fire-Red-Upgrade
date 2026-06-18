#include "src\config.h"

#ifdef SIDEWAY_STAIRS_IMPLEMENTED
.thumb

.equ behaviour_0,   0xB0     @ First of six contiguous, free behaviour bytes

.equ behaviour_1,   (behaviour_0 + 1)
.equ behaviour_2,   (behaviour_1 + 1)
.equ behaviour_3,   (behaviour_2 + 1)
.equ behaviour_4,   (behaviour_3 + 1)
.equ behaviour_5,   (behaviour_4 + 1)

.equ npc_states,    0x02036E38
.equ walkrun_state, 0x02037078

.equ change_movement_return,                    0x0805BB66|1
.equ walk_return,                               0x0805C10E|1
.equ run_return,                                0x0805C156|1
.equ bike_return,                               0x0805C186|1
.equ npc_set_direction,                         0x0805FBDC|1
.equ numbers_move_direction,                    0x08063A20|1
.equ sub_8063F84,                               0x08063F84|1
.equ sub_8064008,                               0x08064008|1
.equ sub_806408C,                               0x0806408C|1
.equ change_movement_2_return,                  0x080BD3D0|1

.equ npc_tile_to,       0x1E
.equ walkrun_npcid,     0x05

.equ left,              0x03
.equ right,             0x04
.equ down_and_left,     0x05
.equ down_and_right,    0x06
.equ up_and_left,       0x07
.equ up_and_right,      0x08

.align 2
sideway_change_movement:
    mov r0, r5
    bl change_movement_main
    add r1, sp, #4
    mov r2, r4
    ldr r3, =(numbers_move_direction)
    bl call_via_r3
    ldr r3, =(change_movement_return)

call_via_r3:
    bx r3

.align 2
sideway_change_movement_2:
    mov r0, r4
    bl change_movement_main
    add r1, sp, #4
    mov r2, r5
    ldr r3, =(numbers_move_direction)
    bl call_via_r3
    ldr r3, =(change_movement_2_return) 
    bx r3

.align 2
change_movement_main:
    push {lr}
    ldrb r0, [r0, #npc_tile_to]

branch_from_behaviour_byte:
    cmp r0, #behaviour_0
    beq behaviour_0_check
    cmp r0, #behaviour_1
    beq behaviour_1_check
    cmp r0, #behaviour_2
    beq behaviour_2_check
    cmp r0, #behaviour_3
    beq behaviour_3_check
    cmp r0, #behaviour_4
    beq behaviour_4_check
    cmp r0, #behaviour_5
    beq behaviour_5_check
    b normal_tile_behaviour

behaviour_0_check:
    cmp r6, #left
    beq move_up_and_left
    cmp r6, #right
    beq move_down_and_right
    b normal_tile_behaviour

behaviour_1_check:
    cmp r6, #left
    beq move_up_and_left
    b normal_tile_behaviour

behaviour_2_check:
    cmp r6, #right
    beq move_down_and_right
    b normal_tile_behaviour

behaviour_3_check:
    cmp r6, #left
    beq move_down_and_left
    cmp r6, #right
    beq move_up_and_right
    b normal_tile_behaviour

behaviour_4_check:
    cmp r6, #right
    beq move_up_and_right
    b normal_tile_behaviour

behaviour_5_check:
    cmp r6, #left
    beq move_down_and_left
    b normal_tile_behaviour

move_down_and_left:
    mov r0, #down_and_left
    b set_player_movement

move_down_and_right:
    mov r0, #down_and_right
    b set_player_movement

move_up_and_left:
    mov r0, #up_and_left
    b set_player_movement

move_up_and_right:
    mov r0, #up_and_right
    b set_player_movement

normal_tile_behaviour:
    mov r0, r6

set_player_movement:
    pop {r3}
    bx r3

.align 2
.pool

sideway_walk:
    push {lr}
    push {r0}
    ldr r0, =(walkrun_state)
    ldrb r1, [r0, #walkrun_npcid]
    lsl r0, r1, #3
    add r0, r0, r1
    lsl r0, r0, #2
    ldr r1, =(npc_states)
    add r0, r0, r1
    ldrb r1, [r0, #npc_tile_to]
    pop {r0}
    lsl r0, r0, #24
    lsr r0, r0, #24
    cmp r1, #behaviour_0
    blt not_walking_on_stair
    cmp r1, #behaviour_5
    bgt not_walking_on_stair
    mov r2, #0
    ldr r3, =(use_table + 1)
    bl call_via_r3
    b walk_end

not_walking_on_stair:
    ldr r3, =(sub_8063F84)
    bl call_via_r3

walk_end:
    ldr r3, =(walk_return)
    bx r3

.align 2
sideway_bike:
    push {lr}
    push {r0}
    ldr r0, =(walkrun_state)
    ldrb r1, [r0, #walkrun_npcid]
    lsl r0, r1, #3
    add r0, r0, r1
    lsl r0, r0, #2
    ldr r1, =(npc_states)
    add r0, r0, r1
    ldrb r1, [r0, #npc_tile_to]
    pop {r0}
    lsl r0, r0, #24
    lsr r0, r0, #24
    cmp r1, #behaviour_0
    blt not_biking_on_stair
    cmp r1, #behaviour_5
    bgt not_biking_on_stair
    mov r2, #1
    ldr r3, =(use_table + 1)
    bl call_via_r3
    b bike_end

not_biking_on_stair:
    ldr r3, =(sub_8064008)
    bl call_via_r3

bike_end:
    ldr r3, =(bike_return)
    bx r3

.align 2
sideway_run:
    push {lr}
    push {r0}
    ldr r0, =(walkrun_state)
    ldrb r1, [r0, #walkrun_npcid]
    lsl r0, r1, #3
    add r0, r0, r1
    lsl r0, r0, #2
    ldr r1, =(npc_states)
    add r0, r0, r1
    ldrb r1, [r0, #npc_tile_to]
    pop {r0}
    lsl r0, r0, #24
    lsr r0, r0, #24
    cmp r1, #behaviour_0
    blt not_running_on_stair
    cmp r1, #behaviour_5
    bgt not_running_on_stair
    mov r2, #2
    ldr r3, =(use_table + 1)
    bl call_via_r3
    b run_end

not_running_on_stair:
    ldr r3, =(sub_806408C)
    bl call_via_r3

run_end:
    ldr r3, =(run_return)
    bx r3

.align 2
use_table:
    push {r4, lr}
    sub sp, #8
    lsl r0, r0, #24
    lsr r4, r0, #24

get_table_from_r2:
    ldr r0, =(table_of_tables)
    lsl r2, r2, #2
    add r2, r2, r0

get_entry_from_behaviour_byte:
    sub r1, #behaviour_0
    lsl r1, r1, #2
    ldr r2, [r2]
    add r1, r1, r2
    ldr r1, [r1]

call_memcpy_with_entry:
    mov r0, sp
    mov r2, #5
    ldr r3, =(memcpy)
    bl call_via_r3
    cmp r4, #4
    bls use_table_end
    mov r4, #0

use_table_end:
    mov r1, sp
    add r0, r1, r4
    ldrb r0, [r0]
    add sp, #8
    pop {r4, pc}

walk_entry_0: .byte 0x10, 0x10, 0x11, 0xAC, 0xAB
walk_entry_1: .byte 0x10, 0x10, 0x11, 0xAC, 0x13
walk_entry_2: .byte 0x10, 0x10, 0x11, 0x12, 0xAB
walk_entry_3: .byte 0x10, 0x10, 0x11, 0xAA, 0xAD
walk_entry_4: .byte 0x10, 0x10, 0x11, 0x12, 0xAD
walk_entry_5: .byte 0x10, 0x10, 0x11, 0xAA, 0x13

bike_entry_0: .byte 0x31, 0x31, 0x32, 0xB4, 0xB3
bike_entry_1: .byte 0x31, 0x31, 0x32, 0xB4, 0x34
bike_entry_2: .byte 0x31, 0x31, 0x32, 0x33, 0xB3
bike_entry_3: .byte 0x31, 0x31, 0x32, 0xB2, 0xB5
bike_entry_4: .byte 0x31, 0x31, 0x32, 0x33, 0xB5
bike_entry_5: .byte 0x31, 0x31, 0x32, 0xB2, 0x34

run_entry_0:  .byte 0x3D, 0x3D, 0x3E, 0xB1, 0xAF
run_entry_1:  .byte 0x3D, 0x3D, 0x3E, 0xB1, 0x40
run_entry_2:  .byte 0x3D, 0x3D, 0x3E, 0x3F, 0xAF
run_entry_3:  .byte 0x3D, 0x3D, 0x3E, 0xAE, 0xB0
run_entry_4:  .byte 0x3D, 0x3D, 0x3E, 0x3F, 0xB0
run_entry_5:  .byte 0x3D, 0x3D, 0x3E, 0xAE, 0x40

.align 2
walk_table: .word (walk_entry_0), (walk_entry_1), (walk_entry_2), (walk_entry_3), (walk_entry_4), (walk_entry_5)
bike_table: .word (bike_entry_0), (bike_entry_1), (bike_entry_2), (bike_entry_3), (bike_entry_4), (bike_entry_5)
run_table: .word (run_entry_0), (run_entry_1), (run_entry_2), (run_entry_3), (run_entry_4), (run_entry_5)
table_of_tables: .word (walk_table), (bike_table), (run_table)

.pool
#endif
