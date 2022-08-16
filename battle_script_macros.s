@Useful Stuff
@Battle Scripts
@.equ FAILED, 0x81D7DF2
.equ NOEFFECT, 0x81D7E04
.equ ABSORB_REBRANCHER, 0x81D694E
.equ BS_MOVE_FAINT, 0x81D6947
.equ BS_MOVE_END, 0x81D694E
.equ BS_MAKE_MOVE_MISS, 0x81D6958
.equ BS_MOVE_MISSED, 0x81D695E
.equ BS_MOVE_MISSED_PAUSE, 0x81D6960
.equ BS_BUFF_ATK_STATS, 0x81D6B9E
.equ BS_MOVE_WEATHER_CHANGE, 0x81D7A14
.equ BS_WEATHER_FORM_CHANGES, 0x81D92DC
.equ BS_FLUSH_MSGBOX, 0x81D96A8
.equ BS_HIT_FROM_ATTACKSTRING, 0x81D692E
.equ BS_HIT_FROM_DAMAGE_CALC, 0x81D6930
.equ BS_HIT_FROM_ATTACKANIMATION, 0x81D6934
.equ BS_STANDARD_HIT, 0x81D6926
.equ BS_FLUSH_MESSAGE_BOX, 0x81D96A8

@String Tables
.equ gStatUpStringIds, 0x83FE57C
.equ gStatDownStringIds, 0x83FE588

@Banks
.equ BANK_TARGET, 0
.equ BANK_ATTACKER, 1
.equ BANK_EFFECT, 2
.equ BANK_FAINTED, 3
.equ BANK_SCRIPTING, 10
.equ BANK_PLAYER_1, 11
.equ BANK_OPPONENT_1, 12
.equ BANK_PLAYER_2, 13
.equ BANK_OPPONENT_2, 14
.equ BANK_SWITCHING, 15

@Comparisons
.equ EQUALS, 0x0
.equ NOTEQUALS, 0x1
.equ GREATERTHAN, 0x2
.equ LESSTHAN, 0x3
.equ ANDS, 0x4
.equ NOTANDS, 0x5

@Delays
.equ DELAY_1SECOND, 0x40
.equ DELAY_HALFSECOND, 0x20

@Booleans
.equ FALSE, 0x0
.equ TRUE, 0x1

@Macros
	.macro calculatedamage
	critcalc
	damagecalc
	typecalc
	adjustnormaldamage
	.endm

	.macro forfeityesnobox bank
	.byte 0x77
	.byte \bank
	.byte 0xD
	.endm

	.macro jumpifmove compare, rom_address
	.byte 0x2a
	.byte EQUALS
	.4byte CURRENT_MOVE
	.2byte \compare
	.4byte \rom_address
	.endm
	
	.macro jumpifnotmove compare, rom_address
	.byte 0x2a
	.byte NOTEQUALS
	.4byte CURRENT_MOVE
	.2byte \compare
	.4byte \rom_address
	.endm
	
	.macro setmoveeffect effect
	.byte 0x2E
	.word EFFECT_BYTE
	.byte \effect
	.endm
	
	.macro setstatchanger stat
	.byte 0x2E
	.word STAT_CHANGE_BYTE
	.byte \stat
	.endm
	
	.macro jumpifmovehadnoeffect rom_address
	.byte 0x29
	.byte ANDS
	.word OUTCOME
	.byte OUTCOME_NO_EFFECT
	.word \rom_address
	.endm
	
	.macro jumpifbehindsubstitute bank rom_address
	.byte 0x1D
	.byte \bank
	.word STATUS2_SUBSTITUTE
	.word \rom_address
	.endm
	
	.macro jumpifweather weather rom_address
	.byte 0x2a
	.byte ANDS
	.word WEATHER_FLAGS
	.hword \weather
	.word \rom_address
	.endm
	
	.macro jumpifabilitypreventsstatloss bank rom_address
	.byte 0x1E
	.byte \bank
	.byte ABILITY_CLEARBODY
	.word \rom_address
	.byte 0x1E
	.byte \bank
	.byte ABILITY_WHITESMOKE
	.word \rom_address
	.byte 0x1E
	.byte \bank
	.byte ABILITY_FULLMETALBODY 
	.word \rom_address
	.endm
	
	.macro jumpifstatcanbelowered bank stat rom_address
	.byte 0xFF, 0x15
	.byte \bank
	.byte \stat
	.byte 0x1 @ATK48_STAT_NEGATIVE
	.4byte \rom_address
	.endm

	.macro jumpifstatcanberaised bank stat rom_address
	.byte 0xFF, 0x15
	.byte \bank
	.byte \stat
	.byte 0x0
	.4byte \rom_address
	.endm
	
	.macro jumpifbattletype battle_type rom_address
	.byte 0x2B
	.byte ANDS
	.word BATTLE_TYPE
	.word \battle_type
	.word \rom_address
	.endm
	
	.macro jumpifnotbattletype battle_type rom_address
	.byte 0x2B
	.byte NOTANDS
	.word BATTLE_TYPE
	.word \battle_type
	.word \rom_address
	.endm	

	.macro copybyte destination source
	.byte 0x31
	.4byte \destination
	.4byte \source
	.byte 0x1
	.endm
	
	.macro copyhword destination source
	.byte 0x31
	.4byte \destination
	.4byte \source
	.byte 0x2
	.endm

	.macro copyword destination source
	.byte 0x31
	.4byte \destination
	.4byte \source
	.byte 0x4
	.endm

	.macro attackcanceler
	.byte 0x00
	.endm

	.macro accuracycheck rom_address, param1
	.byte 0x01
	.4byte \rom_address
	.2byte \param1
	.endm

	.macro attackstring
	.byte 0x02
	.endm

	.macro ppreduce
	.byte 0x03
	.endm

	.macro critcalc
	.byte 0x04
	.endm

	.macro damagecalc
	.byte 0x05
	.endm

	.macro typecalc
	.byte 0x06
	.endm

	.macro adjustnormaldamage
	.byte 0x07
	.endm

	.macro adjustnormaldamage2
	.byte 0x08
	.endm

	.macro attackanimation
	.byte 0x09
	.endm

	.macro waitanimation
	.byte 0x0a
	.endm

	.macro graphicalhpupdate bank
	.byte 0x0b
	.byte \bank
	.endm

	.macro datahpupdate bank
	.byte 0x0c
	.byte \bank
	.endm

	.macro critmessage
	.byte 0x0d
	.endm

	.macro effectivenesssound
	.byte 0x0e
	.endm

	.macro resultmessage
	.byte 0x0f
	.endm

	.macro printstring string
	.byte 0x10
	.2byte \string
	.endm

	.macro printselectionstring string
	.byte 0x11
	.2byte \string
	.endm

	.macro waitmessage delay
	.byte 0x12
	.2byte \delay
	.endm

	.macro printfromtable table
	.byte 0x13
	.4byte \table
	.endm

	.macro printfromtable2 table
	.byte 0x14
	.4byte \table
	.endm

	.macro seteffectwithchancetarget
	.byte 0x15
	.endm

	.macro seteffectprimary @;Ignores Shield Dust & Safeguard
	.byte 0x16
	.endm

	.macro seteffectsecondary
	.byte 0x17
	.endm

	.macro clearstatus bank
	.byte 0x18
	.byte \bank
	.endm

	.macro faintpokemon bank, param2, param3
	.byte 0x19
	.byte \bank
	.byte \param2
	.4byte \param3
	.endm

	.macro dofaintanimation param1
	.byte 0x1a
	.byte \param1
	.endm

	.macro cleareffectsonfaint bank
	.byte 0x1b
	.byte \bank
	.endm

	.macro jumpifstatus bank, status, rom_address
	.byte 0x1c
	.byte \bank
	.4byte \status
	.4byte \rom_address
	.endm

	.macro jumpifsecondarystatus bank, status, rom_address
	.byte 0x1d
	.byte \bank
	.4byte \status
	.4byte \rom_address
	.endm

	.macro jumpifability bank, ability, rom_address
	.byte 0x1e
	.byte \bank
	.byte \ability
	.4byte \rom_address
	.endm

	.macro jumpifsideaffecting bank, status, rom_address
	.byte 0x1f
	.byte \bank
	.2byte \status
	.4byte \rom_address
	.endm

	.macro jumpifstat bank, predicate, statid, quantity, rom_address
	.byte 0x20
	.byte \bank
	.byte \predicate
	.byte \statid
	.byte \quantity
	.4byte \rom_address
	.endm

	.macro jumpifspecialstatusflag bank, mask, status, rom_address
	.byte 0x21
	.byte \bank
	.4byte \mask
	.byte \status
	.4byte \rom_address
	.endm

	.macro jumpiftype bank, type, rom_address
	.byte 0x22
	.byte \bank
	.byte \type
	.4byte \rom_address
	.endm

	.macro getexp bank
	.byte 0x23
	.byte \bank
	.endm

	.macro ifwildbattleend rom_address
	.byte 0x24
	.4byte \rom_address
	.endm

	.macro movevaluescleanup
	.byte 0x25
	.endm

	.macro storeloopingcounter param1
	.byte 0x26
	.byte \param1
	.endm

	.macro decrementmultihit rom_address
	.byte 0x27
	.4byte \rom_address
	.endm

	.macro goto rom_address
	.byte 0x28
	.4byte \rom_address
	.endm

	.macro jumpifbyte predicate, checkaddr, compare, rom_address
	.byte 0x29
	.byte \predicate
	.4byte \checkaddr
	.byte \compare
	.4byte \rom_address
	.endm

	.macro jumpifbytenotequal byte1:req, byte2:req, jumpptr:req
	jumpifarraynotequal \byte1, \byte2, 0x1, \jumpptr
	.endm

	.macro jumpifhalfword predicate, checkaddr, compare, rom_address
	.byte 0x2a
	.byte \predicate
	.4byte \checkaddr
	.2byte \compare
	.4byte \rom_address
	.endm

	.macro jumpifword predicate, checkaddr, compare, rom_address
	.byte 0x2b
	.byte \predicate
	.4byte \checkaddr
	.4byte \compare
	.4byte \rom_address
	.endm

	.macro jumpifarrayequal mem1, mem2, size, rom_address
	.byte 0x2c
	.4byte \mem1
	.4byte \mem2
	.byte \size
	.4byte \rom_address
	.endm

	.macro jumpifarraynotequal mem1, mem2, size, rom_address
	.byte 0x2d
	.4byte \mem1
	.4byte \mem2
	.byte \size
	.4byte \rom_address
	.endm

	.macro setbyte pointer, value
	.byte 0x2e
	.4byte \pointer
	.byte \value
	.endm

	.macro addbyte pointer, value
	.byte 0x2f
	.4byte \pointer
	.byte \value
	.endm

	.macro subtractbyte pointer, value
	.byte 0x30
	.4byte \pointer
	.byte \value
	.endm

	.macro copyarray destination, source, size
	.byte 0x31
	.4byte \destination
	.4byte \source
	.byte \size
	.endm

	.macro copyarraywithindex destination, source, index, size
	.byte 0x32
	.4byte \destination
	.4byte \source
	.4byte \index
	.byte \size
	.endm

	.macro orbyte pointer, value
	.byte 0x33
	.4byte \pointer
	.byte \value
	.endm

	.macro orhalfword pointer, value
	.byte 0x34
	.4byte \pointer
	.2byte \value
	.endm

	.macro orword pointer, value
	.byte 0x35
	.4byte \pointer
	.4byte \value
	.endm

	.macro bicbyte pointer, value
	.byte 0x36
	.4byte \pointer
	.byte \value
	.endm

	.macro bichalfword pointer, value
	.byte 0x37
	.4byte \pointer
	.2byte \value
	.endm

	.macro bicword pointer, value
	.byte 0x38
	.4byte \pointer
	.4byte \value
	.endm

	.macro pause delay
	.byte 0x39
	.2byte \delay
	.endm

	.macro waitstateatk
	.byte 0x3a
	.endm

	.macro healthbarupdate bank
	.byte 0x3b
	.byte \bank
	.endm

	.macro return
	.byte 0x3c
	.endm

	.macro end
	.byte 0x3d
	.endm

	.macro end2
	.byte 0x3e
	.endm

	.macro end3
	.byte 0x3f
	.endm

	.macro jumpifaffectedbyprotect rom_address
	.byte 0x40
	.4byte \rom_address
	.endm

	.macro call rom_address
	.byte 0x41
	.4byte \rom_address
	.endm

	.macro jumpiftype2 bank, type, rom_address
	.byte 0x42
	.byte \bank
	.byte \type
	.4byte \rom_address
	.endm

	.macro jumpifabilitypresent ability, rom_address
	.byte 0x43
	.byte \ability
	.4byte \rom_address
	.endm

	.macro endselectionscript
	.byte 0x44
	.endm

	.macro playanimation bank, animation, var_address
	.byte 0x45
	.byte \bank
	.byte \animation
	.4byte \var_address
	.endm

	.macro playanimation2 bank, mem_address, int
	.byte 0x46
	.byte \bank
	.4byte \mem_address
	.4byte \int
	.endm

	.macro setgraphicalstatchangevalues
	.byte 0x47
	.endm

	.macro playstatchangeanimation bank, color, int
	.byte 0x48
	.byte \bank
	.byte \color
	.byte \int
	.endm

	.macro cmd49 bank, int
	.byte 0x49
	.byte \bank
	.byte \int
	.endm

	.macro typecalc2
	.byte 0x4a
	.endm

	.macro returnatktoball
	.byte 0x4b
	.endm

	.macro getswitchedmondata bank
	.byte 0x4c
	.byte \bank
	.endm

	.macro switchindataupdate bank
	.byte 0x4d
	.byte \bank
	.endm

	.macro switchinanim bank, int
	.byte 0x4e
	.byte \bank
	.byte \int
	.endm

	.macro jumpifcannotswitch bank, rom_address
	.byte 0x4f
	.byte \bank
	.4byte \rom_address
	.endm

	.macro openpartyscreen bank, rom_address
	.byte 0x50
	.byte \bank
	.4byte \rom_address
	.endm

	.macro switchhandleorder bank, param2
	.byte 0x51
	.byte \bank
	.byte \param2
	.endm

	.macro switchineffects bank
	.byte 0x52
	.byte \bank
	.endm

	.macro trainerslidein bank
	.byte 0x53
	.byte \bank
	.endm

	.macro playse effect
	.byte 0x54
	.2byte \effect
	.endm

	.macro fanfare int
	.byte 0x55
	.4byte \int
	.endm

	.macro pokemonfaintcry bank_or_side
	.byte 0x56
	.byte \bank_or_side
	.endm

	.macro flee
	.byte 0x57
	.endm

	.macro returntoball bank
	.byte 0x58
	.byte \bank
	.endm

	.macro handlelearnnewmove param1, param2, bank_maybe
	.byte 0x59
	.4byte \param1
	.4byte \param2
	.byte \bank_maybe
	.endm

	.macro yesnoboxlearnmove rom_address
	.byte 0x5a
	.4byte \rom_address
	.endm

	.macro yesnoboxstoplearningmove rom_address
	.byte 0x5b
	.4byte \rom_address
	.endm

	.macro flash bank
	.byte 0x5c
	.byte \bank
	.endm

	.macro getmoneyreward rom_address
	.byte 0x5d
	.4byte \rom_address
	.endm

	.macro atk5e bank
	.byte 0x5e
	.byte \bank
	.endm

	.macro swapattackerwithtarget
	.byte 0x5f
	.endm

	.macro incrementgamestat int
	.byte 0x60
	.byte \int
	.endm

	.macro drawpartystatussummary bank
	.byte 0x61
	.byte \bank
	.endm

	.macro hidepartystatussummary bank
	.byte 0x62
	.byte \bank
	.endm

	.macro jumptoattack bank
	.byte 0x63
	.byte \bank
	.endm

	.macro statusanimation bank
	.byte 0x64
	.byte \bank
	.endm

	.macro status2animation bank, status2
	.byte 0x65
	.byte \bank
	.4byte \status2
	.endm

	.macro chosenstatusanimation bank_or_side, bank_or_side2, status
	.byte 0x66
	.byte \bank_or_side
	.byte \bank_or_side2
	.4byte \status
	.endm

	.macro yesnobox
	.byte 0x67
	.endm

	.macro cancelallactions
	.byte 0x68
	.endm

	.macro adjustsetdamage
	.byte 0x69
	.endm

	.macro removeitem bank
	.byte 0x6a
	.byte \bank
	.endm

	.macro atknameinbuff1
	.byte 0x6b
	.endm

	.macro drawlvlupbox
	.byte 0x6c
	.endm

	.macro resetsentmonsvalue
	.byte 0x6d
	.endm

	.macro setatktoplayer0
	.byte 0x6e
	.endm

	.macro makevisible bank
	.byte 0x6f
	.byte \bank
	.endm

	.macro recordlastability bank
	.byte 0x70
	.byte \bank
	.endm

	.macro buffermovetolearn
	.byte 0x71
	.endm

	.macro jumpifplayerran rom_address
	.byte 0x72
	.4byte \rom_address
	.endm

	.macro hpthresholds bank
	.byte 0x73
	.byte \bank
	.endm

	.macro hpthresholds2 bank
	.byte 0x74
	.byte \bank
	.endm

	.macro useitemonopponent
	.byte 0x75
	.endm

	.macro various bank, int
	.byte 0x76
	.byte \bank
	.byte \int
	.endm

	.macro setprotect
	.byte 0x77
	.endm

	.macro faintifabilitynotdamp
	.byte 0x78
	.endm

	.macro setuserhptozero
	.byte 0x79
	.endm

	.macro jumpwhiletargetvalid rom_address
	.byte 0x7a
	.4byte \rom_address
	.endm

	.macro setdamageasrestorehalfmaxhp rom_address, int
	.byte 0x7b
	.4byte \rom_address
	.byte \int
	.endm

	.macro jumptolastusedattack
	.byte 0x7c
	.endm

	.macro setrain
	.byte 0x7d
	.endm

	.macro setreflect
	.byte 0x7e
	.endm

	.macro setleechseed
	.byte 0x7f
	.endm

	.macro manipulatedamage id
	.byte 0x80
	.byte \id
	.endm

	.macro setrest rom_address
	.byte 0x81
	.4byte \rom_address
	.endm

	.macro jumpifnotfirstturn rom_address
	.byte 0x82
	.4byte \rom_address
	.endm

	.macro nop3
	.byte 0x83
	.endm

	.macro jumpifcannotsleep rom_address
	.byte 0x84
	.4byte \rom_address
	.endm

	.macro stockpile
	.byte 0x85
	.endm

	.macro stockpiletobasedamage rom_address
	.byte 0x86
	.4byte \rom_address
	.endm

	.macro stockpiletohprecovery rom_address
	.byte 0x87
	.4byte \rom_address
	.endm

	.macro negativedamage
	.byte 0x88
	.endm

	.macro statbuffchange target, rom_address
	.byte 0x89
	.byte \target
	.4byte \rom_address
	.endm

	.macro normalisebuffs
	.byte 0x8a
	.endm

	.macro setbide
	.byte 0x8b
	.endm

	.macro confuseifrepeatingattackends
	.byte 0x8c
	.endm

	.macro setloopcounter count
	.byte 0x8d
	.byte \count
	.endm

	.macro initmultihitstring
	.byte 0x8e
	.endm

	.macro forcerandomswitch bankSwitching bankForcing rom_address
	.byte 0x8f
	.byte \bankSwitching
	.byte \bankForcing
	.4byte \rom_address
	.endm

	.macro changetypestoenemyattacktype rom_address
	.byte 0x90
	.4byte \rom_address
	.endm

	.macro givepaydaymoney
	.byte 0x91
	.endm

	.macro setlightscreen
	.byte 0x92
	.endm

	.macro tryKO rom_address
	.byte 0x93
	.4byte \rom_address
	.endm

	.macro gethalfcurrentenemyhp
	.byte 0x94
	.endm

	.macro setsandstorm
	.byte 0x95
	.endm

	.macro weatherdamage
	.byte 0x96
	.endm

	.macro tryinfatuatebank bank rom_address
	.byte 0x97
	.byte \bank
	.4byte \rom_address
	.endm

	.macro refreshhpbar bank
	.byte 0x98
	.byte \bank
	.endm

	.macro setmisteffect
	.byte 0x99
	.endm

	.macro setincreasedcriticalchance
	.byte 0x9a
	.endm

	.macro transformdataexecution
	.byte 0x9b
	.endm

	.macro setsubstituteeffect
	.byte 0x9c
	.endm

	.macro copyattack rom_address
	.byte 0x9d
	.4byte \rom_address
	.endm

	.macro metronomeeffect
	.byte 0x9e
	.endm

	.macro nightshadedamageeffect
	.byte 0x9f
	.endm

	.macro psywavedamageeffect
	.byte 0xa0
	.endm

	.macro counterdamagecalculator rom_address
	.byte 0xa1
	.4byte \rom_address
	.endm

	.macro mirrorcoatdamagecalculator rom_address
	.byte 0xa2
	.4byte \rom_address
	.endm

	.macro disablelastusedattack bank rom_address
	.byte 0xa3
	.byte \bank
	.4byte \rom_address
	.endm

	.macro setencore rom_address
	.byte 0xa4
	.4byte \rom_address
	.endm

	.macro painsplitdamagecalculator rom_address
	.byte 0xa5
	.4byte \rom_address
	.endm

	.macro settypetorandomresistance rom_address
	.byte 0xa6
	.4byte \rom_address
	.endm

	.macro setalwayshitflag
	.byte 0xa7
	.endm

	.macro copymovepermanently rom_address
	.byte 0xa8
	.4byte \rom_address
	.endm

	.macro trychoosesleeptalkmove rom_address
	.byte 0xa9
	.4byte \rom_address
	.endm

	.macro destinybondeffect
	.byte 0xaa
	.endm

	.macro trysetdestinybondtohappen
	.byte 0xab
	.endm

	.macro remaininghptopower
	.byte 0xac
	.endm

	.macro reducepprandom rom_address
	.byte 0xad
	.4byte \rom_address
	.endm

	.macro healpartystatus
	.byte 0xae
	.endm

	.macro cursetarget rom_address
	.byte 0xaf
	.4byte \rom_address
	.endm

	.macro setspikes rom_address
	.byte 0xb0
	.4byte \rom_address
	.endm

	.macro setforesight
	.byte 0xb1
	.endm

	.macro setperishsong rom_address
	.byte 0xb2
	.4byte \rom_address
	.endm

	.macro rolloutdamagecalculation
	.byte 0xb3
	.endm

	.macro jumpifconfusedandstatmaxed stat, rom_address
	.byte 0xb4
	.byte \stat
	.4byte \rom_address
	.endm

	.macro furycutterdamagecalculation
	.byte 0xb5
	.endm

	.macro happinesstodamagecalculation
	.byte 0xb6
	.endm

	.macro presentdamagecalculation
	.byte 0xb7
	.endm

	.macro setsafeguard
	.byte 0xb8
	.endm

	.macro magnitudedamagecalculation
	.byte 0xb9
	.endm

	.macro jumpifnopursuitswitchdmg rom_address
	.byte 0xba
	.4byte \rom_address
	.endm

	.macro setsunny
	.byte 0xbb
	.endm

	.macro maxattackhalvehp rom_address
	.byte 0xbc
	.4byte \rom_address
	.endm

	.macro copyfoestats rom_address
	.byte 0xbd
	.4byte \rom_address
	.endm

	.macro breakfree
	.byte 0xbe
	.endm

	.macro setcurled
	.byte 0xbf
	.endm

	.macro recoverbasedonsunlight rom_address
	.byte 0xc0
	.4byte \rom_address
	.endm

	.macro hiddenpowerdamagecalculation
	.byte 0xc1
	.endm

	.macro selectnexttarget
	.byte 0xc2
	.endm

	.macro setfutureattack rom_address
	.byte 0xc3
	.4byte \rom_address
	.endm

	.macro trydobeatup rom_address1, rom_address2
	.byte 0xc4
	.4byte \rom_address1
	.4byte \rom_address2
	.endm

	.macro setsemiinvulnerablebit
	.byte 0xc5
	.endm

	.macro clearsemiinvulnerablebit
	.byte 0xc6
	.endm

	.macro setminimize
	.byte 0xc7
	.endm

	.macro sethail
	.byte 0xc8
	.endm

	.macro jumpifattackandspecialattackcannotfall rom_address
	.byte 0xc9
	.4byte \rom_address
	.endm

	.macro setforcedtarget
	.byte 0xca
	.endm

	.macro setcharge
	.byte 0xcb
	.endm

	.macro callterrainattack
	.byte 0xcc
	.endm

	.macro cureifburnedparalysedorpoisoned rom_address
	.byte 0xcd
	.4byte \rom_address
	.endm

	.macro settorment rom_address
	.byte 0xce
	.4byte \rom_address
	.endm

	.macro jumpifnodamage rom_address
	.byte 0xcf
	.4byte \rom_address
	.endm

	.macro settaunt rom_address
	.byte 0xd0
	.4byte \rom_address
	.endm

	.macro sethelpinghand rom_address
	.byte 0xd1
	.4byte \rom_address
	.endm

	.macro itemswap rom_address
	.byte 0xd2
	.4byte \rom_address
	.endm

	.macro copyability rom_address
	.byte 0xd3
	.4byte \rom_address
	.endm

	.macro trywish functype, rom_address
	.byte 0xd4
	.byte \functype
	.4byte \rom_address
	.endm

	.macro setroots rom_address
	.byte 0xd5
	.4byte \rom_address
	.endm

	.macro doubledamagedealtifdamaged
	.byte 0xd6
	.endm

	.macro cansetyawn rom_address
	.byte 0xd7
	.4byte \rom_address
	.endm

	.macro setdamagetohealthdifference rom_address
	.byte 0xd8
	.4byte \rom_address
	.endm

	.macro scaledamagebyhealthratio
	.byte 0xd9
	.endm

	.macro abilityswap rom_address
	.byte 0xda
	.4byte \rom_address
	.endm

	.macro imprisoneffect rom_address
	.byte 0xdb
	.4byte \rom_address
	.endm

	.macro setgrudge rom_address
	.byte 0xdc
	.4byte \rom_address
	.endm

	.macro weightdamagecalculation
	.byte 0xdd
	.endm

	.macro assistattackselect rom_address
	.byte 0xde
	.4byte \rom_address
	.endm

	.macro setmagiccoat rom_address
	.byte 0xdf
	.4byte \rom_address
	.endm

	.macro setstealstatchange rom_address
	.byte 0xe0
	.4byte \rom_address
	.endm

	.macro trygetintimidatetarget rom_address
	.byte 0xe1
	.4byte \rom_address
	.endm

	.macro switchoutabilities bank
	.byte 0xe2
	.byte \bank
	.endm

	.macro jumpiffainted bank, rom_address
	.byte 0xe3
	.byte \bank
	.4byte \rom_address
	.endm

	.macro secretpower
	.byte 0xe4
	.endm

	.macro pickupitemcalculation
	.byte 0xe5
	.endm

	.macro castformtransform
	.byte 0xe6
	.endm

	.macro castformswitch
	.byte 0xe7
	.endm

	.macro settypebasedhalvers rom_address
	.byte 0xe8
	.4byte \rom_address
	.endm

	.macro seteffectbyweather
	.byte 0xe9
	.endm

	.macro recycleitem rom_address
	.byte 0xea
	.4byte \rom_address
	.endm

	.macro settypetoterrain rom_address
	.byte 0xeb
	.4byte \rom_address
	.endm

	.macro pursuitwhenswitched rom_address
	.byte 0xec
	.4byte \rom_address
	.endm

	.macro snatchmove
	.byte 0xed
	.endm

	.macro removereflectlightscreen
	.byte 0xee
	.endm

	.macro pokemoncatchfunction
	.byte 0xef
	.endm

	.macro catchpoke
	.byte 0xf0
	.endm

	.macro trysetcaughtmondexflags rom_address
	.byte 0xf1
	.4byte \rom_address
	.endm

	.macro displaydexinfo
	.byte 0xf2
	.endm

	.macro capturenicknamepoke rom_address
	.byte 0xf3
	.4byte \rom_address
	.endm

	.macro subattackerhpbydmg
	.byte 0xf4
	.endm

	.macro removeattackerstatus1
	.byte 0xf5
	.endm

	.macro finishaction
	.byte 0xf6
	.endm

	.macro finishturn
	.byte 0xf7
	.endm

	.macro callasm asm_address
	.byte 0xf8
	.4byte \asm_address
	.endm

	.macro sethalfword mem_address hword
	.byte 0xf9
	.4byte \mem_address
	.2byte \hword
	.endm

	.macro setword mem_address word
	.byte 0xfa
	.4byte \mem_address
	.4byte \word
	.endm

	.macro setspecialstatusbit bank specialstatus
	.byte 0xfb
	.byte \bank
	.4byte \specialstatus
	.endm

	.macro clearspecialstatusbit bank specialstatus
	.byte 0xfc
	.byte \bank
	.4byte \specialstatus
	.endm

	.macro jumpifabilitypresenttargetfield ability rom_address
	.byte 0xfd
	.byte \ability
	.4byte \rom_address
	.endm

	.macro prefaintmoveendeffects arg
	.byte 0xFE
	.byte \arg, 0x0
	.endm

	.macro cureprimarystatus bank rom_address
	.byte 0xFF, 0x02
	.byte \bank
	.4byte \rom_address
	.endm

	.macro jumpifpartnerattack moveid rom_address
	.byte 0xFF, 0x03
	.byte \moveid
	.4byte \rom_address
	.endm

	.macro setterrain rom_address
	.byte 0xFF, 0x06
	.4byte \rom_address
	.endm

	.macro jumpifhelditemeffect bank effect rom_address
	.byte 0xFF, 0x07
	.byte \bank
	.byte \effect
	.4byte \rom_address
	.endm

	.macro counterclear bank counter_id rom_address
	.byte 0xFF, 0x08
	.byte \bank
	.byte \counter_id
	.4byte \rom_address
	.endm

	.macro jumpifcounter bank counter_id predicate compare_byte rom_address
	.byte 0xFF, 0x09
	.byte \bank
	.byte \counter_id
	.byte \predicate
	.byte \compare_byte
	.4byte \rom_address
	.endm
	
	.macro setability bank ability
	.byte 0xFF, 0x0A
	.byte \bank
	.byte \ability
	.endm
	
	.macro jumpiftargetpartner rom_address
	.byte 0xFF, 0x0C
	.word \rom_address
	.endm
	
	.macro setcounter bank counter_id amount
	.byte 0xFF, 0x0E
	.byte \bank
	.byte \counter_id
	.byte \amount
	.endm
	
	.macro jumpifgrounded bank rom_address
	.byte 0xFF, 0x0F
	.byte \bank
	.4byte \rom_address
	.endm
	
	.macro jumpifhelditem bank item rom_address
	.byte 0xFF, 0x10
	.byte \bank
	.2byte \item
	.4byte \rom_address
	.endm
	
	.macro reloadhealthbar bank
	.byte 0xFF, 0x11
	.byte \bank
	.endm
	
	.macro jumpifhealthcomparestomax bank predicate rom_address
	.byte 0xFF, 0x12
	.byte \bank
	.byte \predicate
	.4byte \rom_address
	.endm

	.macro setdamagetobankhealthfraction bank percent useActualMaxHP
	.byte 0xFF, 0x13
	.byte \bank
	.byte \percent
	.byte \useActualMaxHP
	.endm

	.macro jumpiftypepresent type rom_address
	.byte 0xFF, 0x14
	.byte \type
	.4byte \rom_address
	.endm
	
	.macro jumpifstatcanbemodified bank stat direction rom_address
	.byte 0xFF, 0x15
	.byte \bank
	.byte \stat
	.byte \direction
	.4byte \rom_address
	.endm
	
	.macro jumpifnoviablemonsleft bank rom_address
	.byte 0xFF, 0x16
	.byte \bank
	.4byte \rom_address
	.endm
	
	.macro setsidestatus bank halver
	.byte 0xFF, 0x17
	.byte \bank
	.2byte \halver
	.endm

	.macro clearsidestatus bank halver
	.byte 0xFF, 0x18
	.byte \bank
	.2byte \halver
	.endm
	
	.macro formchange bank original_species target_species reload_type reload_stats reload_ability rom_address
	.byte 0xFF, 0x19
	.byte \bank
	.2byte \original_species
	.2byte \target_species
	.byte \reload_type
	.byte \reload_stats
	.byte \reload_ability
	.4byte \rom_address
	.endm
	
	.macro jumpifabilitypresentattackerfield ability rom_address
	.byte 0xFF, 0x1A
	.byte \ability
	.4byte \rom_address
	.endm
	
	.macro tryactivateswitchinability bank
	.byte 0xFF, 0x1B
	.byte \bank
	.endm
	
	.macro handletrainerslidemsg bank case
	.byte 0xFF, 0x1C
	.byte \bank
	.byte \case
	.endm

	.macro trytrainerslidefirstdownmsg bank
	.byte 0xFF, 0x1D
	.byte \bank
	.endm

	.macro trainerslideout position
	.byte 0xFF, 0x1E
	.byte \position
	.endm
	
	.macro flowershieldlooper move_effect rom_address fail_address
	.byte 0xFF, 0x1F
	.byte \move_effect
	.4byte \rom_address
	.4byte \fail_address
	.endm
	
	.macro jumpifprotectedbycraftyshield bank rom_address
	.byte 0xFF, 0x20
	.byte \bank
	.4byte \rom_address
	.endm
	
	.macro tryspectralthiefsteal rom_address
	.byte 0xFF, 0x21
	.4byte \rom_address
	.endm

	.macro jumpifspecies bank species rom_address
	.byte 0xFF, 0x22
	.byte \bank
	.2byte \species
	.4byte \rom_address
	.endm
	
	.macro faintpokemonaftermove
	.byte 0xFF, 0x23
	.byte 0x0, 0x0
	.endm
	
	.macro jumpifattackeralreadydiddamage rom_address
	.byte 0xFF, 0x24
	.4byte \rom_address
	.endm
	
	.macro jumpifterrainandgrounded terrain bank rom_address
	.byte 0xFF, 0x25
	.byte \terrain
	.byte \bank
	.4byte \rom_address
	.endm
	
	.macro attackstringnoprotean
	.byte 0xFF, 0x26
	.endm
	
	.macro tryactivateprotean
	.byte 0xFF, 0x27
	.endm

	.macro jumpifweight bank predicate weight rom_address
	.byte 0xFF, 0x28
	.byte \bank
	.byte \predicate
	.2byte \weight
	.4byte \rom_address
	.endm

	.macro trysetsleep bank rom_address
	.byte 0xFF, 0x29
	.byte \bank
	.4byte \rom_address
	.endm

	.macro trysetparalysis bank rom_address
	.byte 0xFF, 0x2A
	.byte \bank
	.4byte \rom_address
	.endm

	.macro trysetburn bank rom_address
	.byte 0xFF, 0x2B
	.byte \bank
	.4byte \rom_address
	.endm

	.macro trysetpoison bank rom_address
	.byte 0xFF, 0x2C
	.byte \bank
	.4byte \rom_address
	.endm
	
	.macro addindicatorforattackerswitchineffects
	.byte 0xFF, 0x2D
	.endm
	
	.macro setmoveeffect2
	.byte 0xFF, 0x2E
	.endm

	.macro setmaxmoveeffect
	.byte 0xFF, 0x2F
	.endm

	.macro jumpifdynamaxed bank rom_address
	.byte 0xFF, 0x30
	.byte \bank
	.4byte \rom_address
	.endm

	.macro jumpifraidboss bank rom_address
	.byte 0xFF, 0x31
	.byte \bank
	.4byte \rom_address
	.endm

	.macro recycleberry bankish rom_address
	.byte 0xFF, 0x32
	.byte \bankish
	.4byte \rom_address
	.endm

	.macro seteffectprimaryscriptingbank
	.byte 0xFF, 0x33
	.endm

	.macro canconfuse bank rom_address
	.byte 0xFF, 0x34
	.byte \bank
	.4byte \rom_address
	.endm

	.macro jumpifmaxchistrikecapped bank rom_address
	.byte 0xFF, 0x35
	.byte \bank
	.4byte \rom_address
	.endm

	.macro trygetcottondowntarget rom_address
	.byte 0xFF, 0x36
	.4byte \rom_address
	.endm

@ various command changed to more readable macros
	.equ VARIOUS_CANCEL_MULTI_TURN_MOVES, 0
	.equ VARIOUS_SET_MAGIC_COAT_TARGET, 1
	.equ VARIOUS_IS_RUNNING_IMPOSSIBLE, 2
	.equ VARIOUS_GET_MOVE_TARGET, 3
	.equ VARIOUS_CASE_4, 4
	.equ VARIOUS_RESET_INTIMIDATE_TRACE_BITS, 5
	.equ VARIOUS_UPDATE_CHOICE_MOVE_ON_LVL_UP, 6
	.equ VARIOUS_RESET_PLAYER_FAINTED_FLAG, 7
	.equ VARIOUS_CASE_8, 8
	.equ VARIOUS_RETURN_OPPONENT_MON1, 9
	.equ VARIOUS_RETURN_OPPONENT_MON2, 10
	.equ VARIOUS_CHECK_POKEFLUTE, 11
	.equ VARIOUS_WAIT_FANFARE, 12

	.macro cancelmultiturnmoves battler:req
	various \battler, VARIOUS_CANCEL_MULTI_TURN_MOVES
	.endm

	.macro setmagiccoattarget battler:req
	various \battler, VARIOUS_SET_MAGIC_COAT_TARGET
	.endm

	.macro getifcantrunfrombattle battler:req
	various \battler, VARIOUS_IS_RUNNING_IMPOSSIBLE
	.endm

	.macro getmovetarget battler:req
	various \battler, VARIOUS_GET_MOVE_TARGET
	.endm

	.macro various4 battler:req
	various \battler, 4
	.endm

	.macro resetintimidatetracebits battler:req
	various \battler, VARIOUS_RESET_INTIMIDATE_TRACE_BITS
	.endm

	.macro updatechoicemoveonlvlup battler:req
	various \battler, VARIOUS_UPDATE_CHOICE_MOVE_ON_LVL_UP
	.endm

	.macro resetplayerfaintedflag battler:req
	various \battler, VARIOUS_RESET_PLAYER_FAINTED_FLAG
	.endm

	.macro various8 battler:req
	various \battler, 8
	.endm

	.macro returnopponentmon1toball battler:req
	various \battler, VARIOUS_RETURN_OPPONENT_MON1
	.endm

	.macro returnopponentmon2toball battler:req
	various \battler, VARIOUS_RETURN_OPPONENT_MON2
	.endm

	.macro checkpokeflute battler:req
	various \battler, VARIOUS_CHECK_POKEFLUTE
	.endm

	.macro waitfanfare battler:req
	various \battler, VARIOUS_WAIT_FANFARE
	.endm

.macro tryblockweatherwithprimalweather suffix
	jumpifweather weather_harsh_sun, BattleScript_ExtremelyHarshSunlightWasNotLessened\suffix 
	jumpifweather weather_heavy_rain, BattleScript_NoReliefFromHeavyRain\suffix
	jumpifweather weather_air_current, BattleScript_MysteriousAirCurrentBlowsOn\suffix
	jumpifweather weather_vicious_sandstorm, BattleScript_ViciousSandstormRefusesToLetUp\suffix
.endm
