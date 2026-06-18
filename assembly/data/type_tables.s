.thumb
.global gTypeEffectiveness
.global gMoveMenuInfoIcons

.include "../asm_defines.s"
/*
type_tables.s
	type effectiveness table, type icon definitions
*/

.macro typeicon width height offset
.byte \width
.byte \height
.hword \offset
.endm

gMoveMenuInfoIcons:
typeicon 12, 12, 0x00       @ Unused
typeicon 32, 12, 0x20       @ Normal icon
typeicon 32, 12, 0x64       @ Fight icon
typeicon 32, 12, 0x60       @ Flying icon
typeicon 32, 12, 0x80       @ Poison icon
typeicon 32, 12, 0x48       @ Ground icon
typeicon 32, 12, 0x44       @ Rock icon
typeicon 32, 12, 0x6C       @ Bug icon
typeicon 32, 12, 0x68       @ Ghost icon
typeicon 32, 12, 0x88       @ Steel icon
typeicon 32, 12, 0xA4       @ ??? (Mystery) icon
typeicon 32, 12, 0x24       @ Fire icon
typeicon 32, 12, 0x28       @ Water icon
typeicon 32, 12, 0x2C       @ Grass icon
typeicon 32, 12, 0x40       @ Electric icon
typeicon 32, 12, 0x84       @ Psychic icon
typeicon 32, 12, 0x4C       @ Ice icon
typeicon 32, 12, 0xA0       @ Dragon icon
typeicon 32, 12, 0x8C       @ Dark icon
typeicon 42, 12, 0xA8       @ -Type- icon
typeicon 42, 12, 0xC0       @ -Power- icon
typeicon 42, 12, 0xC8       @ -Accuracy- icon
typeicon 42, 12, 0xE0       @ -PP- icon
typeicon 42, 12, 0xE8       @ -Effect- icon
typeicon 32, 12, 0x100      @ Fairy icon
typeicon 32, 12, 0x104      @ Stellar Icon
typeicon  8,  8, 0xAE       @ Unused (Small white pokeball)
typeicon  8,  8, 0xAF       @ Unused (Small dark pokeball)

