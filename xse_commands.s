@All scripting macros off of XSE and http://sphericalice.com/romhacking/documents/script/index.html

@Message box types
.equ MSG_OBTAIN, 0x0
.equ MSG_FIND, 0x1
.equ MSG_FACE, 0x2
.equ MSG_SIGN, 0x3
.equ MSG_KEEPOPEN, 0x4
.equ MSG_YESNO, 0x5
.equ MSG_NORMAL, 0x6
.equ MSG_POKENAV, 0xA

@Compare values
.equ lessthan, 0x0 @A is less than B
.equ equal, 0x1 @A is equal to B
.equ greaterthan, 0x2 @A is more than B
.equ lessorequal, 0x3 @A is less than or equal to B
.equ greaterorequal, 0x4 @A is greater than or equal to B
.equ notequal, 0x5 @A is not equal to B
.equ false, 0x0
.equ true, 0x1

@azuril13's if command becuase it's nice
.equ _goto,	0x06
.equ _jump, 0x06
.equ _call,	0x07

.macro if condition:req goto_or_call:req script:req
.byte \goto_or_call, \condition
.word \script
.endm

@Commands
@ Does nothing.
.macro nop
.byte 0x00
.endm

@ Does nothing.
.macro nop1
.byte 0x01
.endm

@ Terminates script execution.
.macro end
.byte 0x02
.endm

@ Jumps back to after the last-executed call statement, and continues script execution from there.
.macro return
.byte 0x03
.endm

@ Jumps to destination and continues script execution from there. The location of the calling script is remembered and can be returned to later.
.macro call destination:req
.byte 0x04
.4byte \destination
.endm

@ Jumps to destination and continues script execution from there.
.macro goto destination:req
.byte 0x05
.4byte \destination
.endm

.macro jump destination:req
.byte 0x05
.4byte \destination
.endm

@ If the result of the last comparison matches condition (see Comparison operators), jumps to destination and continues script execution from there.
.macro goto_if condition:req, destination:req
.byte 0x06
.byte \condition
.4byte \destination
.endm

@ If the result of the last comparison matches condition (see Comparison operators), calls destination.
.macro call_if condition:req, destination:req
.byte 0x07
.byte \condition
.4byte \destination
.endm

@ Jumps to the standard function at index function.
.macro gotostd function:req
.byte 0x08
.byte \function
.endm

@ Calls the standard function at index function.
.macro callstd function:req
.byte 0x09
.byte \function
.endm

@ If the result of the last comparison matches condition (see Comparison operators), jumps to the standard function at index function.
.macro gotostd_if condition:req, function:req
.byte 0x0a
.byte \condition
.byte \function
.endm

@ If the result of the last comparison matches condition (see Comparison operators), calls the standard function at index function.
.macro callstd_if condition:req, function:req
.byte 0x0b
.byte \condition
.byte \function
.endm

@ Executes a script stored in a default RAM location.
.macro gotoram
.byte 0x0c
.endm

@ Terminates script execution and "resets the script RAM".
.macro killscript
.byte 0x0d
.endm

@ Sets mystery event status
.macro setmysteryeventstatus value:req
.byte 0x0e
.byte \value
.endm

@ Sets the specified script bank to immediate value.
.macro loadpointer destination:req, value:req
.byte 0x0f
.byte \destination
.4byte \value
.endm

.macro loadword destination:req, value:req
	loadpointer \destination, \value
.endm

@ Sets the specified script bank to immediate value.
.macro loadbyte destination:req, value:req
.byte 0x10
.byte \destination
.byte \value
.endm

@ Sets the byte at offset to value.
.macro writebytetoaddr value:req, offset:req
.byte 0x11
.byte \value
.4byte \offset
.endm

.macro writebytetooffset value:req, offset:req
	writebytetoaddr \value, \offset
.endm

@ Copies the byte value at source into the specified script bank.
.macro loadbytefromaddr destination:req, source:req
.byte 0x12
.byte \destination
.4byte \source
.endm

@ Not sure. Judging from XSE's description I think it takes the least-significant byte in bank source and writes it to destination.
.macro setptrbyte source:req, destination:req
.byte 0x13
.byte \source
.4byte \destination
.endm

@ Copies the contents of bank source into bank destination.
.macro copylocal destination:req, source:req
.byte 0x14
.byte \destination
.byte \source
.endm

@ Copies the byte at source to destination, replacing whatever byte was previously there.
.macro copybyte destination:req, source:req
.byte 0x15
.4byte \destination
.4byte \source
.endm

@ Changes the value of destination to value.
.macro setvar destination:req, value:req
.byte 0x16
.2byte \destination
.2byte \value
.endm

@ Changes the value of destination by adding value to it. Overflow is not prevented (0xFFFF + 1 = 0x0000).
.macro addvar destination:req, value:req
.byte 0x17
.2byte \destination
.2byte \value
.endm

@ Changes the value of destination by subtracting value to it. Overflow is not prevented (0x0000 - 1 = 0xFFFF).
.macro subvar destination:req, value:req
.byte 0x18
.2byte \destination
.2byte \value
.endm

@ Copies the value of source into destination.
.macro copyvar destination:req, source:req
.byte 0x19
.2byte \destination
.2byte \source
.endm

@ If source is not a variable, then this function acts like setvar. Otherwise, it acts like copyvar.
.macro setorcopyvar destination:req, source:req
.byte 0x1a
.2byte \destination
.2byte \source
.endm

.macro copyvarifnotzero destination:req, source:req
	setorcopyvar \destination, \source
.endm

@ Compares the values of script banks a and b, after forcing the values to bytes.
.macro comparelocaltolocal byte1:req, byte2:req
.byte 0x1b
.byte \byte1
.byte \byte2
.endm

@ Compares the least-significant byte of the value of script bank a to a fixed byte value (b).
.macro comparelocaltovalue a:req, b:req
.byte 0x1c
.byte \a
.byte \b
.endm

@ Compares the least-significant byte of the value of script bank a to the byte located at offset b.
.macro comparelocaltoaddr a:req, b:req
.byte 0x1d
.byte \a
.4byte \b
.endm

@ Compares the byte located at offset a to the least-significant byte of the value of script bank b.
.macro compareaddrtolocal a:req, b:req
.byte 0x1e
.4byte \a
.byte \b
.endm

@ Compares the byte located at offset a to a fixed byte value (b).
.macro compareaddrtovalue a:req, b:req
.byte 0x1f
.4byte \a
.byte \b
.endm

.macro comparefarbytetobyte a:req, b:req
	compareaddrtovalue \a, \b
.endm

@ Compares the byte located at offset a to the byte located at offset b.
.macro compareaddrtoaddr a:req, b:req
.byte 0x20
.4byte \a
.4byte \b
.endm
	
.macro comparefarbytes a:req, b:req
	compareaddrtoaddr \a, \b
.endm

@ Compares the value of `var` to a fixed word value (b).
.macro comparevartovalue var:req, value:req
.byte 0x21
.2byte \var
.2byte \value
.endm

.macro compare var:req, value:req
	comparevartovalue \var, \value
.endm

@ Compares the value of `var` to the value of `var2`.
.macro comparevartovar var1:req, var2:req
.byte 0x22
.2byte \var1
.2byte \var2
.endm

.macro comparevars var1:req, var2:req
	comparevartovar \var1, \var2
.endm

@ Calls the native C function stored at `func`.
.macro callasm func:req
.byte 0x23
.4byte \func
.endm

.macro callnative func:req
	callasm \func
.endm

@ Replaces the script with the function stored at `func`. Execution returns to the bytecode script when func returns TRUE.
.macro gotonative func:req
.byte 0x24
.4byte \func
.endm

@ Calls a special function; that is, a piece of ASM code designed for use by scripts and listed in a table of pointers.
.macro special num:req
.byte 0x25
.2byte \num
.endm

@ Calls a special function. That function's output (if any) will be written to the variable you specify.
.macro special2 output:req, num:req
.byte 0x26
.2byte \output
.2byte \num
.endm

.macro specialvar output:req, num:req
	special2 \output, \num
.endm

@ Blocks script execution until a command or ASM code manually unblocks it. Generally used with specific commands and specials. If this command runs, and a subsequent command or piece of ASM does not unblock state, the script will remain blocked indefinitely (essentially a hang).
.macro waitstate
.byte 0x27
.endm

@ Blocks script execution for time frames.
.macro pause time:req
.byte 0x28
.2byte \time
.endm

@ Sets a to 1.
.macro setflag a:req
.byte 0x29
.2byte \a
.endm

@ Sets a to 0.
.macro clearflag a:req
.byte 0x2a
.2byte \a
.endm

@ Compares a to 1.
.macro checkflag a:req
.byte 0x2b
.2byte \a
.endm

@ In FireRed, this command is a nop.
.macro initclock hour:req minute:req
.byte 0x2c
.endm

@ In FireRed, this command is a nop.
.macro dodailyevents
.byte 0x2d
.endm

@ Resets the values of variables VAR_0x8000, VAR_0x8001, and VAR_0x8002.
.macro gettime
.byte 0x2e
.endm

@ Plays the specified (sound_number) sound. Only one sound may play at a time, with newer ones interrupting older ones.
.macro sound sound_number:req
.byte 0x2f
.2byte \sound_number
.endm

.macro playse sound_number:req
	sound \sound_number
.endm

@ Blocks script execution until the currently-playing sound (triggered by sound) finishes playing.
.macro waitse
.byte 0x30
.endm

.macro checksound
.byte 0x30
.endm

@ Plays the specified (fanfare_number) fanfare.
.macro fanfare fanfare_number:req
.byte 0x31
.2byte \fanfare_number
.endm

.macro playfanfare fanfare_number:req
	fanfare \fanfare_number
.endm

@ Blocks script execution until all currently-playing fanfares finish.
.macro waitfanfare
.byte 0x32
.endm

@ Plays the specified (song_number) song. The byte is apparently supposed to be 0x00.
.macro playsong song_number:req, permanent=0
.byte 0x33
.2byte \song_number
.byte \permanent
.endm

.macro playbgm songid:req permanent=0
	playsong \songid \permanent
.endm

@ Plays the specified (song_number) song.
.macro savebgm song_number:req
.byte 0x34
.2byte \song_number
.endm

.macro playsong2 song_number:req
	savebgm \song_number
.endm

@ Crossfades the currently-playing song into the map's default song.
.macro fadedefaultbgm
.byte 0x35
.endm

.macro fadedefault
.byte 0x35
.endm

@ Crossfades the currently-playng song into the specified (song_number) song.
.macro fadesong song_number:req
.byte 0x36
.2byte \song_number
.endm

.macro fadenewbgm song_number:req
	fadesong \song_number
.endm

@ Fades out the currently-playing song.
.macro fadeoutbgm speed:req
.byte 0x37
.byte \speed
.endm

@ Fades the currently-playing song back in.
.macro fadeinbgm speed:req
.byte 0x38
.byte \speed
.endm

@ Sends the player to Warp warp on Map bank.map. If the specified warp is 0xFF, then the player will instead be sent to (x, y) on the map.
.macro warp bank:req map:req warp:req x_axis=0 y_axis=0
.byte 0x39
.byte \bank
.byte \map
.byte \warp
.hword \x_axis
.hword \y_axis
.endm

@ Clone of warp that does not play a sound effect.
.macro warpmuted bank:req map:req warp:req x_axis=0 y_axis=0
.byte 0x3A
.byte \bank
.byte \map
.byte \warp
.hword \x_axis
.hword \y_axis
.endm

@ Clone of warp that uses "a walking effect".
.macro warpwalk bank:req map:req warp:req x_axis=0 y_axis=0
.byte 0x3B
.byte \bank
.byte \map
.byte \warp
.hword \x_axis
.hword \y_axis
.endm

@ Warps the player to another map using a hole animation.
.macro warphole bank:req map:req
.byte 0x3C
.byte \bank
.byte \map
.endm

@ Clone of warp that uses a teleport effect.
.macro warpteleport bank:req map:req warp:req x_axis=0 y_axis=0
.byte 0x3D
.byte \bank
.byte \map
.byte \warp
.hword \x_axis
.hword \y_axis
.endm

@ Clone of warp. Used by a Safari Zone script to return the player to the gatehouse and end the Safari Game. Also used for warpholes.
.macro setwarp bank:req map:req warp:req x_axis=0 y_axis=0
.byte 0x3E
.byte \bank
.byte \map
.byte \warp
.hword \x_axis
.hword \y_axis
.endm

@ Sets a default warp place. If a warp tries to send the player to Warp 127 on Map 127.127, they will instead be sent here. Useful when a map has warps that need to go to script-controlled locations (i.e. elevators).
.macro setdynamicwarp bank:req map:req warp:req x_axis=0 y_axis=0
.byte 0x3F
.byte \bank
.byte \map
.byte \warp
.hword \x_axis
.hword \y_axis
.endm

@ Clone of warp3, except that this writes data to different offsets...
.macro setdivewarp bank:req map:req warp:req x_axis=0 y_axis=0
.byte 0x40
.byte \bank
.byte \map
.byte \warp
.hword \x_axis
.hword \y_axis
.endm

@ Clone of warp3, except that this writes data to different offsets...
.macro setholewarp bank:req map:req warp:req x_axis=0 y_axis=0
.byte 0x41
.byte \bank
.byte \map
.byte \warp
.hword \x_axis
.hword \y_axis
.endm

@ Retrieves the player's zero-indexed X- and Y-coordinates in the map, and stores them in the specified variables.
.macro getplayerpos x:req, y:req
.byte 0x42
.2byte \x
.2byte \y
.endm

.macro getplayerxy x:req, y:req
	getplayerpos x, y
.endm

@ Retrieves the number of Pokemon in the player's party, and stores that number in VAR_RESULT.
.macro countpokemon
.byte 0x43
.endm

.macro getpartysize
	countpokemon
.endm

@ Attempts to add quantity of item index to the player's Bag. If the player has enough room, the item will be added and VAR_RESULT will be set to TRUE; otherwise, VAR_RESULT is set to FALSE.
.macro additem index:req, quantity=1
.byte 0x44
.2byte \index
.2byte \quantity
.endm

@ Removes quantity of item index from the player's Bag.
.macro removeitem index:req, quantity=1
.byte 0x45
.2byte \index
.2byte \quantity
.endm

@ Checks if the player has enough space in their Bag to hold quantity more of item index. Sets VAR_RESULT to TRUE if there is room, or FALSE is there is no room.
.macro checkitemspace index:req, quantity:req
.byte 0x46
.2byte \index
.2byte \quantity
.endm

@ Checks if the player has quantity or more of item index in their Bag. Sets VAR_RESULT to TRUE if the player has enough of the item, or FALSE if they have fewer than quantity of the item.
.macro checkitem index:req, quantity:req
.byte 0x47
.2byte \index
.2byte \quantity
.endm

@ Checks which Bag pocket the specified item belongs in, and writes the pocket value (POCKET_*) to VAR_RESULT. This script is used to show the name of the proper Bag pocket when the player receives an item via callstd (simplified to giveitem in XSE).
.macro checkitemtype index:req
.byte 0x48
.2byte \index
.endm

@ Adds a quantity amount of item index to the player's PC. Both arguments can be variables.
.macro addpcitem index:req, quantity:req
.byte 0x49
.2byte \index
.2byte \quantity
.endm

@ Checks for quantity amount of item index in the player's PC. Both arguments can be variables.
.macro checkpcitem index:req, quantity:req
.byte 0x4a
.2byte \index
.2byte \quantity
.endm

@ In FireRed, this command is a nop. (The argument is read, but not used for anything.)
.macro adddecor decoration:req
.byte 0x4b
.2byte \decoration
.endm

@ In FireRed, this command is a nop. (The argument is read, but not used for anything.)
.macro removedecor decoration:req
.byte 0x4c
.2byte \decoration
.endm

@ In FireRed, this command is a nop. (The argument is read, but not used for anything.)
.macro hasdecor decoration:req
.byte 0x4d
.2byte \decoration
.endm

@ In FireRed, this command is a nop. (The argument is read, but not used for anything.)
.macro checkdecor decoration:req
.byte 0x4e
.2byte \decoration
.endm

@ Applies the movement data at movements to the specified (index) Object. Also closes any standard message boxes that are still open.
@ If no map is specified, then the current map is used.
.macro applymovement index:req, movements:req, mapGroup, mapNum
	.ifb \mapGroup
		.byte 0x4f
		.2byte \index
		.4byte \movements
	.else
		.byte 0x50
		.2byte \index
		.4byte \movements
		.byte \mapGroup
		.byte \mapNum
	.endif
.endm

@ Blocks script execution until the movements being applied to the specified (index) Object finish. If the specified Object is 0x0000, then the command will block script execution until all Objects affected by applymovement finish their movements. If the specified Object is not currently being manipulated with applymovement, then this command does nothing.
@ If no map is specified, then the current map is used.
.macro waitmovement index:req, mapGroup, mapNum
	.ifb \mapGroup
		.byte 0x51
		.2byte \index
	.else
		.byte 0x52
		.2byte \index
		.byte \mapGroup
		.byte \mapNum
	.endif
.endm

@ Attempts to hide the specified (localId) Object on the specified (map_group, map_num) map, by setting its visibility flag if it has a valid one. If the Object does not have a valid visibility flag, this command does nothing.
@ If no map is specified, then the current map is used.
.macro removeobject localId:req, mapGroup, mapNum
	.ifb \mapGroup
		.byte 0x53
		.2byte \localId
	.else
		.byte 0x54
		.2byte \localId
		.byte \mapGroup
		.byte \mapNum
	.endif
.endm

.macro hidesprite localId:req
	removeobject \localId
.endm

.macro hidespriteonmap localId:req, mapGroup:req, mapNum:req
	removeobject \localId, \mapGroup, \mapNum
.endm

@ Attempts to show the specified (localId) Object on the specified (map_group, map_num) map
.macro addobject localId:req, mapGroup, mapNum
	.ifb \mapGroup
		.byte 0x55
		.2byte \localId
	.else
		.byte 0x56
		.2byte \localId
		.byte \mapGroup
		.byte \mapNum
	.endif
.endm

.macro showsprite localId:req
	addobject \localId
.endm

.macro showspriteonmap localId:req, mapGroup:req, mapNum:req
	addobject \localId, \mapGroup, \mapNum
.endm

@ Sets the specified (index) Object's position on the current map.
.macro movesprite index:req, x:req, y:req
.byte 0x57
.2byte \index
.2byte \x
.2byte \y
.endm

.macro setobjectxy index:req, x:req, y:req
	movesprite \index, \x, \y
.endm

@ unknown
.macro showobject index:req, map:req
.byte 0x58
.2byte \index
map \map
.endm

@ unknown
.macro hideobject index:req, map:req
.byte 0x59
.2byte \index
map \map
.endm

@ If the script was called by an Object, then that Object will turn to face toward the metatile that the player is standing on.
.macro faceplayer
.byte 0x5a
.endm

@ Faces the object in a given direction
.macro spriteface localId:req, direction:req
.byte 0x5b
.2byte \localId
.byte \direction
.endm

.macro turnobject localId:req, direction:req
	spriteface \localId, \direction
.endm

@ If the Trainer flag for Trainer index is not set, this command does absolutely nothing.
.macro trainerbattle0 type:req index:req filler:req intro:req loss:req
.byte 0x5C
.byte 0x0
.hword \index
.hword \filler
.word \intro
.word \loss
.endm

.macro trainerbattle1 type:req index:req filler:req intro:req loss:req extra:req
.byte 0x5C
.byte 0x1
.hword \index
.hword \filler
.word \intro
.word \loss
.word \extra
.endm

.macro trainerbattle2 type:req index:req filler:req intro:req loss:req extra:req
.byte 0x5C
.byte 0x2
.hword \index
.hword \filler
.word \intro
.word \loss
.word \extra
.endm

.macro trainerbattle3 type:req index:req filler:req intro:req
.byte 0x5C
.byte 0x3
.hword \index
.hword \filler
.word \intro
.endm

.macro trainerbattle4 type:req index:req filler:req intro:req loss:req cantbattle:req
.byte 0x5C
.byte 0x4
.hword \index
.hword \filler
.word \intro
.word \loss
.word \cantbattle
.endm

.macro trainerbattle5 type:req index:req filler:req intro:req loss:req
.byte 0x5C
.byte 0x5
.hword \index
.hword \filler
.word \intro
.word \loss
.endm

.macro trainerbattle6 type:req index:req filler:req intro:req loss:req extra:req extra2:req
.byte 0x5C
.byte 0x6
.hword \index
.hword \filler
.word \intro
.word \loss
.word \extra
.word \extra2
.endm

.macro trainerbattle7 type:req index:req filler:req intro:req loss:req extra:req
.byte 0x5C
.byte 0x7
.hword \index
.hword \filler
.word \intro
.word \loss
.word \extra
.endm

.macro trainerbattle8 type:req index:req filler:req intro:req loss:req extra:req extra2:req
.byte 0x5C
.byte 0x8
.hword \index
.hword \filler
.word \intro
.word \loss
.word \extra
.word \extra2
.endm

.macro trainerbattle9 type:req index:req filler:req win:req loss:req
.byte 0x5C
.byte 0x9
.hword \index
.hword \filler
.word \win
.word \loss
.endm

@trainerbattle 0xA FOE_1_ID FOE_2_ID PARTNER_ID PARTNER_BACKSPRITE_ID 0x0 DEFEAT_TEXT_A DEFEAT_TEXT_B
.macro trainerbattle10 type:req foeindex1:req foeindex2:req partnerindex:req partnerbackspriteindex:req filler:req loss1:req loss2:req
.byte 0x5C
.byte 10
.hword \foeindex1
.hword \foeindex2
.hword \partnerindex
.hword \partnerbackspriteindex
.hword \filler
.word \loss1
.word \loss2
.endm

@trainerbattle 0xB FOE_1_ID FOE_2_ID FOE_1_NPC_ID FOE_2_NPC_ID 0x0 INTRO_TEXT_A INTRO_TEXT_B DEFEAT_TEXT_A DEFEAT_TEXT_B CANNOT_BATTLE_TEXT_A CANNOT_BATTLE_TEXT_B
.macro trainerbattle11 type:req foeindex1:req foeindex2:req foenpcid1:req foenpcid2:req filler:req intro1:req intro2:req loss1:req loss2:req cannotbattle1:req cannotbattle2:req
.byte 0x5C
.byte 11
.hword \foeindex1
.hword \foeindex2
.byte \foenpcid1
.byte \foenpcid2
.hword \filler
.word \intro1
.word \intro2
.word \loss1
.word \loss2
.word \cannotbattle1
.word \cannotbattle2
.endm

@trainerbattle 0xC FOE_ID PARTNER_ID PARTNER_BACKSPRITE_ID 0x0 DEFEAT_TEXT_A
.macro trainerbattle12 type:req foeindex:req partnerindex:req partnerbackspriteindex:req filler:req loss:req
.byte 0x5C
.byte 12
.hword \foeindex
.hword \partnerindex
.hword \partnerbackspriteindex
.hword \filler
.word \loss
.endm

@same as trainerbattle0
.macro trainerbattle13 type:req index:req filler:req intro:req loss:req
.byte 0x5C
.byte 13
.hword \index
.hword \filler
.word \intro
.word \loss
.endm

@same as trainerbattle4
.macro trainerbattle14 type:req index:req filler:req intro:req loss:req cantbattle:req
.byte 0x5C
.byte 14
.hword \index
.hword \filler
.word \intro
.word \loss
.word \cantbattle
.endm

@same as trainerbattle3
.macro trainerbattle15 type:req index:req filler:req intro:req
.byte 0x5C
.byte 15
.hword \index
.hword \filler
.word \intro
.endm

@same as trainerbattle11
.macro trainerbattle16 type:req foeindex1:req foeindex2:req foenpcid1:req foenpcid2:req filler:req intro1:req intro2:req loss1:req loss2:req cannotbattle1:req cannotbattle2:req
.byte 0x5C
.byte 16
.hword \foeindex1
.hword \foeindex2
.byte \foenpcid1
.byte \foenpcid2
.hword \filler
.word \intro1
.word \intro2
.word \loss1
.word \loss2
.word \cannotbattle1
.word \cannotbattle2
.endm

@ Starts a trainer battle using the battle information stored in RAM (usually by trainerbattle, which actually calls this command behind-the-scenes), and blocks script execution until the battle finishes.
.macro battlebegin
.byte 0x5d
.endm

.macro ontrainerbattleend
.byte 0x5e
.endm

.macro ontrainerbattleendgoto
.byte 0x5f
.endm

@ Compares Flag (trainer + 0x500) to 1. (If the flag is set, then the trainer has been defeated by the player.)
.macro checktrainerflag trainer:req
.byte 0x60
.2byte \trainer
.endm

@ Sets Flag (trainer + 0x500).
.macro settrainerflag trainer:req
.byte 0x61
.2byte \trainer
.endm

@ Clears Flag (trainer + 0x500).
.macro cleartrainerflag trainer:req
.byte 0x62
.2byte \trainer
.endm

@ Sets the specified (Object's saved position on the current map.
.macro movesprite2 localId:req, x:req, y:req
.byte 0x63
.2byte \localId
.2byte \x
.2byte \y
.endm

.macro setobjectxyperm localId:req, x:req, y:req
	setobjectxyperm \localId, \x, \y
.endm

@ Sets the specified Object's position to somewhere off the screen
.macro forceupdatepos localId:req
.byte 0x64
.2byte \localId
.endm

.macro moveoffscreen localId:req
	forceupdatepos \localId
.endm

.macro moveobjectoffscreen localId:req
	forceupdatepos \localId
.endm

@ Sets the specified Object's movement behaviour
.macro spritebehave localId:req, byte:req
.byte 0x65
.2byte \localId
.byte \byte
.endm

.macro setobjectmovementtype localId:req, byte:req
	spritebehave \localId, \byte
.endm

@ If a standard message box (or its text) is being drawn on-screen, this command blocks script execution until the box and its text have been fully drawn.
.macro waitmsg
.byte 0x66
.endm

.macro waitmessage
	waitmsg
.endm

@ Starts displaying a standard message box containing the specified text. If text is a pointer, then the string at that offset will be loaded and used. If text is script bank 0, then the value of script bank 0 will be treated as a pointer to the text. (You can use loadpointer to place a string pointer in a script bank.)
.macro preparemsg text:req
.byte 0x67
.4byte \text
.endm

.macro message text:req
	preparemsg \text
.endm

@ Holds the current message box open until the player presses a key. The message box is then closed.
.macro closeonkeypress
.byte 0x68
.endm

.macro closemessage
	closeonkeypress
.endm

@ Ceases movement for all Objects on-screen.
.macro lockall
.byte 0x69
.endm

@ If the script was called by an Object, then that Object's movement will cease.
.macro lock
.byte 0x6a
.endm

@ Resumes normal movement for all Objects on-screen, and closes any standard message boxes that are still open.
.macro releaseall
.byte 0x6b
.endm

@ If the script was called by an Object, then that Object's movement will resume. This command also closes any standard message boxes that are still open.
.macro release
.byte 0x6c
.endm

@ Blocks script execution until the player presses any key.
.macro waitkeypress
.byte 0x6d
.endm

.macro waitbuttonpress
	waitkeypress
.endm

@ Displays a YES/NO multichoice box at the specified coordinates, and blocks script execution until the user makes a selection. Their selection is stored in VAR_RESULT as NO (0) or YES (1). Pressing B is equivalent to answering NO
.macro yesnobox x:req, y:req
.byte 0x6e
.byte \x
.byte \y
.endm

@ Displays a multichoice box from which the user can choose a selection, and blocks script execution until a selection is made. Lists of options are predefined and the one to be used is specified with list. If b is set to a non-zero value, then the user will not be allowed to back out of the multichoice with the B button.
.macro multichoice x:req, y:req, list:req, b:req
.byte 0x6f
.byte \x
.byte \y
.byte \list
.byte \b
.endm

@ Displays a multichoice box from which the user can choose a selection, and blocks script execution until a selection is made. Lists of options are predefined and the one to be used is specified with list. The default argument determines the initial position of the cursor when the box is first opened; it is zero-indexed, and if it is too large, it is treated as 0x00. If b is set to a non-zero value, then the user will not be allowed to back out of the multichoice with the B button.
.macro multichoicedefault x:req, y:req, list:req, default:req, b:req
.byte 0x70
.byte \x
.byte \y
.byte \list
.byte \default
.byte \b
.endm

@ Displays a multichoice box from which the user can choose a selection, and blocks script execution until a selection is made. Lists of options are predefined and the one to be used is specified with list. The per_row argument determines how many list items will be shown on a single row of the box.
.macro multichoicegrid x:req, y:req, list:req, per_row:req, b:req
.byte 0x71
.byte \x
.byte \y
.byte \list
.byte \per_row
.byte \b
.endm

@ No idea
.macro drawbox
.byte 0x72
.endm

@ No idea
.macro erasebox byte1:req, byte2:req, byte3:req, byte4:req
.byte 0x73
.byte \byte1
.byte \byte2
.byte \byte3
.byte \byte4
.endm

@ No idea
.macro drawboxtext
.byte 0x74
.endm

@ Displays a box containing the front sprite for the specified (species) Pokemon species.
.macro showpokepic species:req, x=0xA, y=0x3
.byte 0x75
.2byte \species
.byte \x
.byte \y
.endm

.macro drawmonpic species:req, x=0xA, y=0x3
	showpokepic \species, \x, \y
.endm

@ Hides all boxes displayed with drawmonpic.
.macro hidepokepic
.byte 0x76
.endm

.macro erasemonpic
	hidepokepic
.endm

@ Same as fadescreen but works properly in rain
.macro fadescreenswapbuffers effect:req
.byte 0x77
.byte \effect
.endm

@ Displays the string at pointer as braille text in a standard message box. The string must be formatted to use braille characters.
.macro braillemessage text:req
.byte 0x78
.4byte \text
.endm

@ Gives the player one of the specified (species) Pokemon at level level holding item.
.macro givepokemon species:req level:req item:req unknown1=0 unknown2=0 unknown3=0
.byte 0x79
.hword \species
.byte \level
.hword \item
.word \unknown1
.word \unknown2
.byte \unknown3
.endm

@ Gives the player an Egg of the given species
.macro giveegg species:req
.byte 0x7a
.2byte \species
.endm

@ Sets a party member's move
.macro setmonmove partyId:req, pos:req, move:req
.byte 0x7b
.byte \partyId
.byte \pos
.2byte \move
.endm

@ Checks if at least one Pokemon in the player's party knows the specified (index) attack. If so, VAR_RESULT is set to the (zero-indexed) slot number of the first Pokemon that knows the move. If not, VAR_RESULT is set to PARTY_SIZE. VAR_0x8004 is also set to this Pokemon's species.
.macro checkattack index:req
.byte 0x7c
.2byte \index
.endm

.macro checkpartymove index:req
	checkattack \index
.endm

@ Writes the name of the Pokemon at index species to the specified buffer.
.macro bufferpokemon out:req, species:req
.byte 0x7d
.byte \out
.2byte \species
.endm

.macro getspeciesname out:req, species:req
.byte 0x7d
.byte \out
.2byte \species
.endm

@ Writes the name of the species of the first Pokémon in the player's party to the specified buffer.
.macro bufferfirstpokemon out:req
.byte 0x7e
.byte \out
.endm
	
.macro getfirstpartymonname out:req
.byte 0x7e
.byte \out
.endm

@ Writes the nickname of the Pokemon in slot slot (zero-indexed) of the player's party to the specified buffer. If an empty or invalid slot is specified, ten spaces ("") are written to the buffer.
.macro bufferpartypokemon out:req, slot:req
.byte 0x7f
.byte \out
.2byte \slot
.endm

.macro getpartymonname out:req, slot:req
.byte 0x7f
.byte \out
.2byte \slot
.endm

@ Writes the name of the item at index item to the specified buffer. If the specified index is larger than the number of items in the game (0x176), the name of item 0 ("????????") is buffered instead.
.macro bufferitem out:req, item:req
.byte 0x80
.byte \out
.2byte \item
.endm

.macro getitemname out:req, item:req
.byte 0x80
.byte \out
.2byte \item
.endm

@ Writes the name of the decoration at index decoration to the specified buffer. In FireRed, this command is a nop.
.macro bufferdecoration out:req, decoration:req
.byte 0x81
.byte \out
.2byte \decoration
.endm

.macro getdecorname out:req, decoration:req
.byte 0x81
.byte \out
.2byte \decoration
.endm

@ Writes the name of the move at index move to the specified buffer.
.macro bufferattack out:req, move:req
.byte 0x82
.byte \out
.2byte \move
.endm

.macro getmovename out:req, move:req
.byte 0x82
.byte \out
.2byte \move
.endm

@ Converts the value of input to a decimal string, and writes that string to the specified buffer.
.macro buffernumber out:req, input:req
.byte 0x83
.byte \out
.2byte \input
.endm

.macro getnumberstring out:req, input:req
.byte 0x83
.byte \out
.2byte \input
.endm

@ Writes the standard string identified by index to the specified buffer. This command has no protections in place at all, so specifying an invalid standard string (e.x. 0x2B) can and usually will cause data corruption.
.macro bufferstd out:req, index:req
.byte 0x84
.byte \out
.2byte \index
.endm

.macro getstdstring out:req, index:req
.byte 0x84
.byte \out
.2byte \index
.endm

@ Copies the string at offset to the specified buffer.
.macro bufferstring out:req, offset:req
.byte 0x85
.byte \out
.4byte \offset
.endm

.macro getstring out:req, offset:req
.byte 0x85
.byte \out
.4byte \offset
.endm

@ Opens the Pokemart system, offering the specified products for sale.
.macro pokemart products:req
.byte 0x86
.4byte \products
.endm

@ Opens the Pokemart system and treats the list of items as decorations.
.macro pokemartdecor products:req
.byte 0x87
.4byte \products
.endm

@ Apparent clone of pokemart.
.macro pokemartbp products:req
.byte 0x88
.4byte \products
.endm

@ Starts up the slot machine minigame.
.macro playslotmachine word:req
.byte 0x89
.2byte \word
.endm

@ In FireRed, this command is a nop.
.macro plantberrytree
.byte 0x8a
.endm

@ In FireRed, this command sets the byte at 0x03000EA8 to 0x01. I do not know what that means.
.macro choosecontestpkmn
.byte 0x8b
.endm

@ In FireRed, this command is a nop.
.macro startcontest
.byte 0x8c
.endm

@ In FireRed, this command is a nop.
.macro showcontestresults
.byte 0x8d
.endm

@ In FireRed, this command is a nop.
.macro contestlinktransfer
.byte 0x8e
.endm

@ Stores a random integer between 0 and limit in VAR_RESULT.
.macro random limit:req
.byte 0x8f
.2byte \limit
.endm

@ If check is 0x00, this command adds value to the player's money.
.macro addmoney value:req, check=0
.byte 0x90
.4byte \value
.byte \check
.endm

@ If check is 0x00, this command subtracts value from the player's money.
.macro removemoney value:req, check=0
.byte 0x91
.4byte \value
.byte \check
.endm

@ If check is 0x00, this command will check if the player has money >= value; VAR_RESULT is set to TRUE if the player has enough money, or FALSE if they do not.
.macro checkmoney value:req, check=0
.byte 0x92
.4byte \value
.byte \check
.endm

@ Spawns a secondary box showing how much money the player has.
.macro showmoney x:req, y:req, check=0
.byte 0x93
.byte \x
.byte \y
.byte \check
.endm

.macro showmoneybox x:req, y:req, check=0
	showmoney \x, \y, \check
.endm

@ Hides the secondary box spawned by showmoney.
@ The two arguments are unused.
@ They are retained here for backwards compatibility with Ruby/Sapphire.
.macro hidemoney x=0, y=0
.byte 0x94
.byte \x
.byte \y
.endm

.macro hidemoneybox x=0, y=0
	hidemoney \x, \y
.endm

@ Updates the secondary box spawned by showmoney. Consumes but does not use arguments.
.macro updatemoney x=0, y=0, check=0
.byte 0x95
.byte \x
.byte \y
.byte \check
.endm

.macro updatemoneybox x=0, y=0, check=0
	updatemoney \x, \y, \check
.endm

@ In FireRed, this command is a nop.
.macro getpokenewsactive
.byte 0x96
.endm

@ Fades the screen to black or back, using the specified effect. Effect 0x00 fades in, and effect 0x01 fades out.
.macro fadescreen effect:req
.byte 0x97
.byte \effect
.endm

@ Fades the screen to and from black and white. Mode 0x00 fades from black, mode 0x01 fades out to black, mode 0x2 fades in from white, and mode 0x3 fades out to white.
.macro fadescreenspeed effect:req, speed:req
.byte 0x98
.byte \effect
.byte \speed
.endm

.macro setflashlevel word:req
.byte 0x99
.2byte \word
.endm

.macro setflashradius word:req
	setflashlevel \word
.endm

.macro animateflash byte:req
.byte 0x9a
.byte \byte
.endm

.macro messageautoscroll pointer:req
.byte 0x9b
.4byte \pointer
.endm

@ Executes the specified field move animation.
.macro doanimation animation:req
.byte 0x9c
.2byte \animation
.endm

.macro dofieldeffect animation:req
	doanimation \animation
.endm

@ Sets up the field effect argument argument with the value value.
.macro setanimation argument:req, param:req
.byte 0x9d
.byte \argument
.2byte \param
.endm

.macro setfieldeffectarg argument:req, param:req
	setanimation \argument, \param
.endm

@ Blocks script execution until all playing field move animations complete.
.macro waitfieldeffect animation:req
.byte 0x9e
.2byte \animation
.endm

.macro waitanimation checkanimation_param:req
.byte 0x9E
.hword \checkanimation_param
.endm

@ Sets which healing place the player will return to if all of the Pokemon in their party faint.
.macro sethealingplace heallocation:req
.byte 0x9f
.2byte \heallocation
.endm

.macro setrespawn heallocation:req
	sethealingplace \heallocation
.endm

@ Checks the player's gender. If male, then MALE (0) is stored in VAR_RESULT. If female, then FEMALE (1) is stored in VAR_RESULT.
.macro checkgender
.byte 0xa0
.endm

.macro checkplayergender
	checkgender
.endm

@ Plays the specified (species) Pokemon's cry. You can use waitcry to block script execution until the sound finishes.
.macro cry species:req, effect:req
.byte 0xa1
.2byte \species
.2byte \effect
.endm

.macro playmoncry species:req, effect:req
	cry \species, \effect
.endm

@ Changes the metatile at (x, y) on the current map.
.macro setmaptile x:req, y:req, metatile_number:req, passable:req
.byte 0xA2
.hword \x
.hword \y
.hword \metatile_number
.hword \passable
.endm

.macro setmetatile x:req, y:req, metatile_number:req, passable:req
	setmaptile \x, \y, \metatile_number, \passable
.endm

@ Queues a weather change to the default weather for the map.
.macro resetweather
.byte 0xa3
.endm

@ Queues a weather change to type weather.
.macro setweather type:req
.byte 0xa4
.2byte \type
.endm

@ Executes the weather change queued with resetweather or setweather. The current weather will smoothly fade into the queued weather.
.macro doweather
.byte 0xa5
.endm

@ This command manages cases in which maps have tiles that change state when stepped on (specifically, cracked/breakable floors).
.macro setstepcallback subroutine:req
.byte 0xa6
.byte \subroutine
.endm

.macro setmaplayoutindex index:req
.byte 0xa7
.2byte \index
.endm

.macro setmapfooter setmapfooter_param:req
.byte 0xA7
.hword \setmapfooter_param
.endm

.macro setobjectsubpriority index:req, bank:req, map:req, priority:req
.byte 0xa8
.2byte \index
.byte \bank
.byte \map
.byte \priority
.endm

.macro resetobjectsubpriority index:req, bank:req, map:req
.byte 0xa9
.byte \bank
.byte \map
map \map
.endm

.macro createsprite sprite:req, localId:req, x:req, y:req, elevation:req, direction:req
.byte 0xaa
.byte \sprite
.byte \localId
.2byte \x
.2byte \y
.byte \elevation
.byte \direction
.endm

.macro createvobject sprite:req, localId:req, x:req, y:req, elevation:req, direction:req
	createsprite \sprite, \localId, \x, \y, \elevation, \direction
.endm

.macro spritefacevirtual index:req, direction:req
.byte 0xab
.byte \index
.byte \direction
.endm

.macro turnvobject index:req, direction:req
	spritefacevirtual \index, \direction
.endm

@ Opens the door metatile at (X, Y) with an animation.
.macro opendoor x:req, y:req
.byte 0xac
.2byte \x
.2byte \y
.endm

@ Closes the door metatile at (X, Y) with an animation.
.macro closedoor x:req, y:req
.byte 0xad
.2byte \x
.2byte \y
.endm

@ Waits for the door animation started with opendoor or closedoor to finish.
.macro waitdooranim
.byte 0xae
.endm

@ Sets the door tile at (x, y) to be open without an animation.
.macro setdooropen x:req, y:req
.byte 0xaf
.2byte \x
.2byte \y
.endm

@ Sets the door tile at (x, y) to be closed without an animation.
.macro setdoorclosed x:req, y:req
.byte 0xb0
.2byte \x
.2byte \y
.endm

@ In FireRed, this command is a nop.
.macro addelevmenuitem
.byte 0xb1
.endm

@ In FireRed and Emerald, this command is a nop.
.macro showelevmenu
.byte 0xb2
.endm

.macro checkcoins out:req
.byte 0xb3
.2byte \out
.endm

.macro checkexpandedcoins amount:req
.byte 0xB3
.word \amount
.endm

.macro addcoins count:req
.byte 0xb4
.2byte \count
.endm

.macro removecoins word:req
.byte 0xb5
.2byte \word
.endm

@ Prepares to start a wild battle against a species at Level level holding item. Running this command will not affect normal wild battles. You start the prepared battle with dowildbattle.
.macro setwildbattle species:req, level:req, item=0
.byte 0xb6
.2byte \species
.byte \level
.2byte \item
.endm

@ Starts a wild battle against the Pokemon generated by setwildbattle. Blocks script execution until the battle finishes.
.macro dowildbattle
.byte 0xb7
.endm

.macro setvaddress long:req
.byte 0xb8
.4byte \long
.endm

.macro vgoto pointer:req
.byte 0xb9
.4byte \pointer
.endm

.macro vcall pointer:req
.byte 0xba
.4byte \pointer
.endm

.macro vgoto_if byte:req, pointer:req
.byte 0xbb
.byte \byte
.4byte \pointer
.endm

.macro vcall_if byte:req, pointer:req
.byte 0xbc
.byte \byte
.4byte \pointer
.endm

.macro vmessage pointer:req
.byte 0xbd
.4byte \pointer
.endm

.macro vloadptr pointer:req
.byte 0xbe
.4byte \pointer
.endm

.macro vbufferstring byte:req, pointer:req
.byte 0xbf
.byte \byte
.4byte \pointer
.endm

@ Spawns a secondary box showing how many Coins the player has.
.macro showcoins x=0, y=0
.byte 0xc0
.byte \x
.byte \y
.endm

.macro showcoinsbox x=0, y=0
	showcoins \x, \y
.endm

@ Hides the secondary box spawned by showcoins. It consumes its arguments but doesn't use them.
.macro hidecoins x=0, y=0
.byte 0xC1
.byte \x
.byte \y
.endm

.macro hidecoinsbox x=0, y=0
	hidecoins \x, \y
.endm

@ Updates the secondary box spawned by showcoins. It consumes its arguments but doesn't use them.
.macro updatecoins x=0, y=0
.byte 0xC2
.byte \x
.byte \y
.endm

.macro updatecoinsbox x=0, y=0
	updatecoins \x, \y
.endm

@ Increases the value of the specified game stat by 1. The stat's value will not be allowed to exceed 0x00FFFFFF.
.macro incrementgamestat stat:req
.byte 0xc3
.byte \stat
.endm

@ Sets the destination that using an Escape Rope or Dig will take the player to.
.macro setescapewarp map:req, warp:req, x:req, y:req
.byte 0xc4
map \map
.byte \warp
.2byte \x
.2byte \y
.endm

@ Blocks script execution until cry finishes.
.macro waitcry
.byte 0xc5
.endm

.macro waitmoncry
	waitcry
.endm

@ Writes the name of the specified (box) PC box to the specified buffer.
.macro bufferboxname out:req, box:req
.byte 0xc6
.byte \out
.2byte \box
.endm

@ Sets the color of the text in standard message boxes. 0x00 produces blue (male) text, 0x01 produces red (female) text, 0xFF resets the color to the default for the current OW's gender, and all other values produce black text.
.macro textcolor color:req
.byte 0xc7
.byte \color
.endm

@ The exact purpose of this command is unknown, but it is related to the blue help-text box that appears on the bottom of the screen when the Main Menu is opened.
.macro loadhelp pointer:req
.byte 0xc8
.4byte \pointer
.endm

@ The exact purpose of this command is unknown, but it is related to the blue help-text box that appears on the bottom of the screen when the Main Menu is opened.
.macro unloadhelp
.byte 0xc9
.endm

@ After using this command, all standard message boxes will use the signpost frame.
.macro signmsg
.byte 0xca
.endm

.macro msgboxsign
.byte 0xCA
.endm

@ Ends the effects of signmsg, returning message box frames to normal.
.macro normalmsg
.byte 0xcb
.endm

.macro msgboxnormal
.byte 0xCB
.endm

@ Compares the value of a hidden variable to a dword.
.macro comparehiddenvar a:req, value:req
.byte 0xcc
.byte \a
.4byte \value
.endm

@ Makes the Pokemon in the specified slot of the player's party obedient. It will not randomly disobey orders in battle.
.macro setmonobedient slot:req
.byte 0xcd
.2byte \slot
.endm

@ Checks if the Pokemon in the specified slot of the player's party is obedient. If the Pokemon is disobedient, VAR_RESULT is TRUE. If the Pokemon is obedient (or if the specified slot is empty or invalid), VAR_RESULT is FALSE.
.macro checkmonobedience slot:req
.byte 0xce
.2byte \slot
.endm

@ Causes script execution to jump to the offset specified by the pointer at 0x020375C0.
.macro execram
.byte 0xcf
.endm

@ Sets worldmapflag to 1. This allows the player to Fly to the corresponding map, if that map has a flightspot.
.macro setworldmapflag worldmapflag:req
.byte 0xd0
.2byte \worldmapflag
.endm

@ Alternate warpteleport. Spins the player in on entry.
.macro warpspinenter map:req, warp:req, x:req, y:req
.byte 0xd1
map \map
.byte \warp
.2byte \x
.2byte \y
.endm

@ Changes the location where the player caught the Pokemon in the specified slot of their party.
.macro setmonmetlocation slot:req, location:req
.byte 0xd2
.2byte \slot
.byte \location
.endm

.macro getbraillestringwidth pointer:req
.byte 0xd3
.4byte \pointer
.endm

.macro bufferitemnameplural out:req, item:req, quantity:req
.byte 0xd4
.byte \out
.2byte \item
.2byte \quantity
.endm


@Macro macros, from XSE.
.macro msgbox pointer:req type:req
	loadpointer 0x0 \pointer
	callstd \type
.endm

.macro switch var:req
	copyvar 0x8000, \var
.endm

.macro case condition:req, dest:req
	compare 0x8000, \condition
	if equal _goto \dest
.endm

.macro giveitem item:req amount:req msgtype:req
	copyvarifnotzero 0x8000 \item
	copyvarifnotzero 0x8001 \amount
	callstd \msgtype
.endm

.macro finditem item:req amount:req
	giveitem \item, \amount, MSG_FIND
.endm

.macro obtainitem item:req amount:req
	giveitem \item, \amount, MSG_OBTAIN
.endm

.macro putitemaway item:req, amount=1
	copyvarifnotzero 0x8000 \item
	copyvarifnotzero 0x8001 \amount
	callstd 0x8 @STD_PUT_ITEM_AWAY
.endm

.macro giveitemwithfanfare item:req amount:req song:req
	copyvarifnotzero 0x8000 \item
	copyvarifnotzero 0x8001 \amount
	copyvarifnotzero 0x8002 \song
	callstd 0x9 @STD_RECEIVED_ITEM
.endm

.macro wildbattle species:req level:req item:req
	setwildbattle \species \level \item
	dowildbattle
.endm

/*.macro braillemessage_wait text:req
	setvar 0x8006, 0
	braillemessage \text
	getbraillestringwidth \text
	call EventScript_BrailleCursorWaitButton
.endm*/

.macro multichoiceoption text:req num:req
	setvar 0x8006 \num
	loadpointer 0x0 \text
	special 0x25
.endm

.macro comparedayofweek day:req
	comparefarbytetobyte 0x3005EA5 \day
.endm

.macro comparehour hour:req
	comparefarbytetobyte 0x3005EA6 \hour
.endm

.macro candodailyevent var:req
	setvar 0x8000 \var
	setvar 0x8001 0x0
	special2 LASTRESULT 0xA0
.endm

.macro setdailyevent var:req
	setvar 0x8000 \var
	setvar 0x8001 0x1
	special2 LASTRESULT 0xA0
.endm

.macro hidemappopup
.byte 0xb1
.endm

.macro startmining
.byte 0xb2
.endm

.macro mapscript tag:req pointer:req
.byte \tag
.word \pointer
.endm

.macro levelscript var:req val:req script:req
.hword \var, \val
.word \script
.endm
