.thumb
.text
.align 2

.include "..\\Anim_Defines.asm"
.include "..\\asm_defines.s"

.global PlayAnimationTable	


.word 0x81d5b88	@ ANIM_NOTHING		original 0x1c6ea8
.word 0x81d5bbe	@ ANIM_STAT_BUFF
.word 0x81d5bc7	@ ANIM_SUBSTITUTE_REMOVAL
.word 0x81d5c04	@ ANIM_CREATE_SUBSTITUTE
.word 0x81d5c0c	@ ANIM_POKEBLOCK_TRANSFORM
.word 0x81d5c54	@ ANIM_KNOCK_OFF_ITEM
.word 0x81d5c5f	@ ANIM_WRAP
.word 0x81d5df2	@ ANIM_ITEM_USE
.word 0x81d5e66	@ ANIM_SMOKE_BALL
.word 0x81d5f42	@ ANIM_CHARGE
.word 0x81d5f8f	@ ANIM_RAIN
.word 0x81d5fd8	@ ANIM_SUN
.word 0x81d5fdd	@ ANIM_SANDSTORM
.word 0x81d5fe2	@ ANIM_HAIL
.word 0x81d5fe7	@ ANIM_LEECH_SEED_HEAL
.word 0x81d5ff5	@ ANIM_HIT
.word 0x81d6026	@ ANIM_STEAL_ITEM
.word 0x81d604b	@ ANIM_WIGGLE
.word 0x81d60a9	@ ANIM_FUTURE_SIGHT_HIT
.word 0x81d6108	@ ANIM_DOOM_DESIRE_HIT
.word 0x81d61cd	@ ANIM_FOCUS_PUNCH_PUMP
.word 0x81d620d	@ ANIM_INGRAIN_RECOVER
.word 0x81d6250	@ ANIM_DARK_SCREEN_RECOVER
.word 0x81d628a	@ ANIM_ASTONISH_DROPS
.word 0x81d6301	@ ANIM_SCARY_FACE_ASTONISH
.word 0x81d637b	@ ANIM_TRANSFORM_FRONT
.word 0x81d6394	@ ANIM_TURN_INTO_ROCK
.word 0x81d63dc	@ ANIM_WAITING_WAGGLE
.word 0x81d64b6	@ ANIM_LEVEL_UP_SHINE
.word 0x81d64d7	@ ANIM_CALL_BACK_POKEMON
.word 0x81d64e8	@ ANIM_CALL_BACK_POKEMON_2
.word 0x81d64f9	@ ANIM_CAPTURE_POKEMON
.word 0x81d657b	@ ANIM_TURN_INTO_POKEBALL
.word 0x81d6594	@ ANIM_SWITCH
.word 0x81d659e	@ ANIM_CREATE_SUBSTITUTE_2
.word ANIM_TRANSFORM
.word ANIM_WISHIWASHI_FISH
.word ANIM_ZYGARDE_CELL_SWIRL
.word ANIM_DELTA_STREAM
.word ANIM_ELECTRIC_SURGE
.word ANIM_GRASSY_SURGE
.word ANIM_MISTY_SURGE
.word ANIM_PSYCHIC_SURGE
.word ANIM_SEA_OF_FIRE
.word ANIM_HEALING_SPARKLES
.word ANIM_LUNAR_DANCE_HEAL
.word ANIM_HEALING_WISH_HEAL
.word ANIM_RED_PRIMAL_REVERSION
.word ANIM_BLUE_PRIMAL_REVERSION
.word ANIM_GRASSY_TERRAIN_HEAL
.word ANIM_POWDER_EXPLOSION
.word ANIM_BEAK_BLAST_WARM_UP
.word ANIM_SHELL_TRAP_SET
.word ANIM_BERRY_EAT
.word ANIM_FOG
.word ANIM_AQUA_RING_HEAL
.word ELECTRIC_TERRAIN_ACTIVE_ANIM
.word GRASSY_TERRAIN_ACTIVE_ANIM
.word MISTY_TERRAIN_ACTIVE_ANIM
.word PSYCHIC_TERRAIN_ACTIVE_ANIM
.word BATON_PASS_ANIM
.word DRAGON_TAIL_BLOW_AWAY_ANIM
.word ANIM_ZMOVE_ACTIVATE
.word ANIM_MEGA_EVOLUTION
.word ANIM_ULTRA_BURST


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_TRANSFORM:
	endanimation


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_WISHIWASHI_FISH:



@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_ZYGARDE_CELL_SWIRL:


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_DELTA_STREAM:


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_ELECTRIC_SURGE:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_GRASSY_SURGE:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_MISTY_SURGE:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_PSYCHIC_SURGE:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_SEA_OF_FIRE:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_HEALING_SPARKLES:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_LUNAR_DANCE_HEAL:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_HEALING_WISH_HEAL:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_RED_PRIMAL_REVERSION:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_BLUE_PRIMAL_REVERSION:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_GRASSY_TERRAIN_HEAL:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_POWDER_EXPLOSION:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_BEAK_BLAST_WARM_UP:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_SHELL_TRAP_SET:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_BERRY_EAT:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_FOG:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_AQUA_RING_HEAL:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ELECTRIC_TERRAIN_ACTIVE_ANIM:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
GRASSY_TERRAIN_ACTIVE_ANIM:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
MISTY_TERRAIN_ACTIVE_ANIM:

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
PSYCHIC_TERRAIN_ACTIVE_ANIM:


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
BATON_PASS_ANIM:


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
DRAGON_TAIL_BLOW_AWAY_ANIM:


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_ZMOVE_ACTIVATE:


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_MEGA_EVOLUTION:


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_ULTRA_BURST:




